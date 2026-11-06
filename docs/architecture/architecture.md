# Software Architecture

This document defines the software architecture of the **DOOM port for the STM32F7508-DK**, built on `doomgeneric`, FreeRTOS (CMSIS-RTOS V2), FatFS, and the STM32F7 HAL/BSP driver stack.

---

## 1. Architectural Overview & Layered Design

The firmware is structured into five horizontal layers, decoupling the platform-agnostic 1993 DOOM engine from the STM32 Cortex-M7 hardware peripherals.

```text
+-----------------------------------------------------------------------------------+
|                            1. CORE GAME ENGINE LAYER                              |
|  +-----------------------------------------------------------------------------+  |
|  |                     doomgeneric (libs/doomgeneric)                          |  |
|  |  • Game Loop & Logic (d_main, g_game)    • 2.5D Renderer (r_bsp, r_plane)   |  |
|  |  • WAD Loader (w_wad)                    • Zone Memory Allocator (z_zone)   |  |
|  +-----------------------------------------------------------------------------+  |
+-----------------------------------------------------------------------------------+
                                        |
                         [ doomgeneric Port Interface ]
             (DG_Init, DG_DrawFrame, DG_GetKey, DG_SleepMs, DG_GetTicksMs)
                                        v
+-----------------------------------------------------------------------------------+
|                        2. PORT ADAPTER & INPUT LAYER (app/)                       |
|  +--------------------------------------+  +-----------------------------------+  |
|  |     doomgeneric_stm32.c / .h         |  |          buttons.c / .h           |  |
|  |  • FreeRTOS Task (doomgeneric_task)  |  |  • 9-Key Gamepad Binding Table    |  |
|  |  • Frame Scaling / Centering Logic   |  |  • State-to-Event XOR Poller      |  |
|  |  • Engine & POSIX Stubs (mkdir, etc) |  |  • Active-Low Pull-Up GPIO Logic  |  |
|  +--------------------------------------+  +-----------------------------------+  |
+-----------------------------------------------------------------------------------+
                                        |
+-----------------------------------------------------------------------------------+
|                      3. RTOS, FILESYSTEM & SYSTEM SERVICES                        |
|  +------------------------+  +------------------------+  +---------------------+  |
|  |   FreeRTOS (CMSIS V2)  |  |     FatFS (ff.c)       |  |  System & Logging   |  |
|  |  • Task Scheduler      |  |  • SD Card Mount       |  |  • Custom _sbrk()   |  |
|  |  • OS Tick & Delays    |  |  • doom1.wad Streaming |  |  • lwprintf / Logs  |  |
|  +------------------------+  +------------------------+  +---------------------+  |
+-----------------------------------------------------------------------------------+
                                        |
+-----------------------------------------------------------------------------------+
|                        4. BOARD SUPPORT PACKAGE (BSP) & HAL                       |
|  +------------------+  +------------------+  +----------------+  +-------------+  |
|  |   LCD & DMA2D    |  |    SD / SDMMC    |  |   FMC SDRAM    |  |    GPIO     |  |
|  |  • BSP_LCD_*     |  |  • sd_diskio.c   |  |  • BSP_SDRAM_* |  |  • HAL_GPIO |  |
|  |  • BSP_DrawFrame |  |  • DMA2 Rx/Tx    |  |  • Init Config |  |             |  |
|  +------------------+  +------------------+  +----------------+  +-------------+  |
+-----------------------------------------------------------------------------------+
                                        |
+-----------------------------------------------------------------------------------+
|                       5. STM32F7508-DK PHYSICAL HARDWARE                          |
|  [ Cortex-M7 + D-Cache ]  [ 480x272 LTDC LCD ]  [ External SDRAM ]  [ MicroSD ]   |
+-----------------------------------------------------------------------------------+
```

### Layer Responsibilities
1. **Core Game Engine (`libs/doomgeneric`):** Executes unmodified game rules, WAD file lump caching (`w_wad.c`), Zone Memory allocation (`z_zone.c`), and 2.5D software rendering into an 8-bit `320x200` indexed framebuffer (`I_VideoBuffer` / `DG_ScreenBuffer`).
2. **Port Adapter & Input Layer (`app/src/doomgeneric`, `app/src/buttons`):**
   * `doomgeneric_stm32.c`: Hosts the FreeRTOS entry task (`doomgeneric_task`), implements the five `DG_*` port callbacks, maintains Cortex-M7 Data Cache (`D-Cache`) coherency, and provides POSIX/engine stubs (`mkdir`, unused video/mouse hooks).
   * `buttons.c`: Abstracts the hardware GPIO tactile buttons (`button_binding_t`) and converts raw active-low GPIO pin states into discrete press/release events.
3. **RTOS, Filesystem & System Services (`core/`, `third_party/`):**
   * **Bootloader (`bootloader/`):** Initializes the external QSPI Flash in memory-mapped mode (`0x90000000`) and jumps to the main application firmware.
   * **FreeRTOS:** Schedules tasks and provides millisecond tick timing via `osKernelGetTickCount()` and `osDelay()`.
   * **FatFS & Newlib Syscalls:** Bridges standard C file operations (`fopen`, `fread`, `fseek`) to the MicroSD card to stream `doom1.wad`.
   * **Custom Memory Allocator (`sysmem.c`):** Overrides `_sbrk()` to route all dynamic `malloc()` / `calloc()` allocations into external SDRAM with strict 8-byte alignment.
   * **Logging (`logger.h`, `lcd_log.c`):** Provides ANSI color-coded UART logging via `lwprintf` alongside on-screen LCD status logging.
4. **Board Support Package (BSP) & HAL (`board/`, `drivers/`):** Configures LTDC display layers, DMA2D hardware color conversion, FMC external SDRAM timings, and DMA-backed SDMMC1 block transfers.

---

## 2. Memory Architecture & Linker Partitioning

Because the STM32F7508 has only 64 KB of internal Flash and 320 KB of total internal SRAM (64 KB DTCM + 256 KB SRAM), the linker script (`STM32F750xx_QSPI_FLASH.ld`) partitions code and data across four distinct physical memory regions:

| Memory Region | Base Address | Allocated Size | Architectural Role & Contents |
| :--- | :--- | :--- | :--- |
| **QSPI Flash** | `0x90000000` | 16 MB | `.text` (Application code ~450 KB) and `.rodata` (Constants). Bootloader resides in internal Flash at `0x08000000`. |
| **DTCMRAM** | `0x20000000` | 64 KB | Zero-wait-state tightly coupled RAM hosting the **FreeRTOS Heap (`ucHeap`)** (~15 KB) and critical interrupt/task stacks. |
| **Internal SRAM** | `0x20010000` | 256 KB | Standard `.data` (pre-initialized structures like `mobjinfo`, `S_music`, `S_sfx` copied before `main()`) and small `.bss` globals. |
| **External SDRAM** | `0xC0000000` | 8 MB (Accessible) | Connected via FMC. Partitioned into three non-overlapping zones to prevent display tearing and heap corruption. |

### External SDRAM Sub-Partitioning (`0xC0000000` – `0xC07FFFFF`)

```text
0xC0000000 +-------------------------------------------------------------------+
           | ZONE 1: LTDC Physical LCD Framebuffer (1 MB Reserved)             |
           | • fbPtr @ 0xC0000000 (480x272 ARGB8888 = ~522 KB)                 |
0xC0100000 +-------------------------------------------------------------------+
           | ZONE 2: Static Uninitialized Engine Buffers (.sdram_data, 1 MB)   |
           | • Marked (NOLOAD) in linker script (__SDRAM_MEMORY__)             |
           | • Renderer arrays: visplanes (r_plane.c), drawsegs, vissprites    |
0xC0200000 +-------------------------------------------------------------------+
           | ZONE 3: Dynamic Application Heap (_sbrk in sysmem.c, 6 MB Cap)    |
           | • 8-byte aligned heap_end pointer                                 |
           | • DOOM Zone Memory (-mb 4 = 4 MB)                                 |
           | • WAD directory cache (lumpinfo_t) & DG_ScreenBuffer              |
0xC07FFFFF +-------------------------------------------------------------------+
```

> **Hardware Constraint Note:**
> 1. **8 MB Physical Boundary:** Due to FMC pin routing on the STM32F7508-DK, only the lower 8 MB (`0xC0000000` – `0xC07FFFFF`) of the onboard SDRAM chip is addressable. DOOM's Zone Memory is explicitly capped at 4 MB via `{"doom", "-mb", "4"}` to prevent BusFaults.
> 2. **Pre-Main Initialization Rule:** Only *uninitialized* globals may be placed in `.sdram_data` (`__SDRAM_MEMORY__`). Pre-initialized arrays (`= { ... }`) must remain in internal SRAM `.data` because `startup_stm32f750xx.s` runs before `BSP_SDRAM_Init()` enables the FMC peripheral.

---

## 3. Core Runtime Data Pipelines

### A. Graphics & DMA2D Hardware-Accelerated Rendering Pipeline
To achieve smooth frame rates without overloading the Cortex-M7 CPU, palette lookup and pixel format conversion are offloaded to the **DMA2D (Chrom-Art Accelerator)**:

```text
[ DOOM Software Renderer ]
       │  (Draws 320x200 8-bit indexed pixels)
       ▼
[ I_VideoBuffer (320x200, L8) ]
       │  Copied / scaled during I_FinishUpdate() -> DG_DrawFrame()
       ▼
[ DG_ScreenBuffer / stretched_buffer (SDRAM) ]
       │
       ▼
[ SCB_CleanDCache_by_Addr() ]  <-- Flushes Cortex-M7 D-Cache lines to physical SDRAM
       │
       ▼
[ DMA2D Hardware Engine (BSP_DrawFrame) ]
  • Mode: DMA2D_M2M_PFC (Pixel Format Conversion: L8 -> ARGB8888)
  • Hardware CLUT: 256-entry RGB palette loaded via HAL_DMA2D_CLUTLoad()
  • Output Offset / Scaling: Centers or stretches frame onto 480x272 panel
       │
       ▼
[ Physical LCD Framebuffer (fbPtr @ 0xC0000000) ]
       │
       ▼
[ LTDC Peripheral -> 480x272 RK043FN48H LCD Panel ]
```

### B. Non-Blocking Input Pipeline (`buttons.c` -> `DG_GetKey`)
DOOM processes input via an event queue (discrete Key-Down and Key-Up transitions) rather than level-triggered polling:

1. **GPIO Hardware Layer:** Nine tactile switches connect GPIO pins across `GPIOB`, `GPIOG`, `GPIOH`, and `GPIOI` to Ground (`GPIO_MODE_INPUT` with `GPIO_PULLUP`).
2. **Bitmask Aggregation (`buttons_get_state`):** Reads all 9 pins and packs active-low states into a 32-bit state bitmask (`FIRE_KEY`, `ACTIVATE_KEY`, `RUN_KEY`, `FORWARD_KEY`, `BACKWARD_KEY`, `LEFT_KEY`, `RIGHT_KEY`, `WEAPON_DOWN_KEY`, `WEAPON_UP_KEY`).
3. **XOR Edge Detector (`DG_GetKey`):**
   * Computes `uint32_t changed = current_button_state ^ previous_button_state;`.
   * Iterates bit-by-bit to isolate the first button whose state flipped.
   * Writes the corresponding `DOOMKEYS` constant (`KEY_FIRE`, `KEY_USE`, `KEY_ENTER`, `KEY_UPARROW`, etc.) to `*doom_key` and the new state (`1` or `0`) to `*pressed`.
   * Updates only that single bit in `previous_button_state` and returns `1`, allowing DOOM to drain simultaneous multi-button presses across consecutive loop iterations without ghosting.

### C. Storage & WAD Streaming Pipeline
1. **Mount & Verification:** Inside `doomgeneric_task`, FatFS mounts the MicroSD card (`f_mount`) and verifies `doom1.wad` in the root directory.
2. **DMA Block Transfers:** When the engine requests map geometry, sprites, or sound lumps via `W_CacheLumpNum()`, FatFS invokes `SD_read()` -> `BSP_SD_ReadBlocks_DMA()`.
3. **Interrupt-Driven Completion:** `SDMMC1_IRQHandler`, `DMA2_Stream3_IRQHandler` (RX), and `DMA2_Stream6_IRQHandler` (TX) handle transfer completion asynchronously, preventing the RTOS scheduler from stalling during disk I/O.

---

## 4. System Boot & Initialization Sequence

Because external memories (QSPI Flash and FMC SDRAM) must be online before code execution or dynamic allocation occurs, the system follows a strict boot sequence:

```text
[ Power-On Reset ]
       │
       ▼
[ 1. Bootloader (0x08000000) ]
  • Initializes Clocks & QSPI Flash in Memory-Mapped Mode (0x90000000)
  • Relocates VTOR and jumps to Application Reset_Handler
       │
       ▼
[ 2. Startup Assembly (startup_stm32f750xx.s) ]
  • Copies initialized .data (mobjinfo, S_music, S_sfx) from QSPI to Internal SRAM
  • Zeroes internal SRAM .bss section
       │
       ▼
[ 3. Application Hardware Init (main.c) ]
  • Enables Cortex-M7 I-Cache & D-Cache
  • HAL_Init() & SystemClock_Config() (216 MHz)
  • BSP_SDRAM_Init() -> Brings 0xC0000000 online (MUST run before malloc)
  • Peripheral & FatFS driver init (GPIO, USART, SDMMC1, DMA2D, LTDC)
       │
       ▼
[ 4. FreeRTOS Kernel Start ]
  • osKernelInitialize() -> Creates doomgeneric_task (8 KB stack) -> osKernelStart()
       │
       ▼
[ 5. Game Task & Engine Bring-Up (doomgeneric_stm32.c) ]
  • Zeroes .sdram_data NOLOAD renderer buffers (memset visplanes, drawsegs, etc.)
  • Mounts MicroSD card via f_mount()
  • DG_Init(): Configures GPIO buttons, initializes LTDC Layer 0 @ 0xC0000000, enables DMA2D
  • doomgeneric_Create(3, {"doom", "-mb", "4"}): Allocates 4 MB Zone Heap & loads doom1.wad
       │
       ▼
[ 6. Main Game Loop ]
  • Infinite while(1) calling doomgeneric_Tick()
```

---

## 5. Repository Directory Mapping

| Directory | Architectural Layer | Description |
| :--- | :--- | :--- |
| `bootloader/` | Boot / Hardware | Standalone CMake project that configures QSPI memory-mapped mode and jumps to `0x90000000`. |
| `app/` | Port Adapter & Input | Custom application code: `doomgeneric_stm32.c`, `buttons.c`, `lcd_log.c`, and `main.c`. |
| `core/` | System Services | STM32CubeMX peripheral init, `sysmem.c` (SDRAM `_sbrk`), `syscalls.c`, and startup assembly. |
| `board/` | BSP Layer | Modified STM32F7508-Discovery BSP drivers (`lcd`, `sd`, `sdram`) with custom `BSP_DrawFrame` DMA2D routines. |
| `libs/doomgeneric/` | Core Game Engine | Forked `doomgeneric` engine submodule with `__SDRAM_MEMORY__` section tags on large renderer arrays. |
| `libs/lwprintf/` | System Services | Lightweight printf library used by the color-coded UART logger. |
| `third_party/` | Middleware | FreeRTOS (CMSIS-RTOS V2) and FatFS filesystem source code. |
| `tests/` | Verification | Ceedling + Unity + CMock off-target unit test suite for `buttons` and `doomgeneric_stm32` logic. |