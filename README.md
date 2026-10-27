# STM32F7508-DK Doom Port

This project is a hardware-accelerated port of [doomgeneric](https://github.com/ozkl/doomgeneric) for the STM32F7508-DK Discovery Board. It leverages the STM32's advanced hardware peripherals—including the DMA2D graphic accelerator, FMC external SDRAM, and SDMMC with DMA to run the classic Doom engine smoothly on a microcontroller.

https://github.com/user-attachments/assets/e0355555-e895-42bd-9704-132b6425685e

## Prerequisites

### Ubuntu / Debian (`apt`)
```bash
sudo apt update
sudo apt install cmake ninja-build gcc-arm-none-eabi libnewlib-arm-none-eabi git stlink-tools
```

### Arch Linux (`pacman`)
```bash
sudo pacman -S cmake ninja arm-none-eabi-gcc arm-none-eabi-newlib git stlink
```

### macOS (`brew`)
```bash
brew install cmake ninja arm-none-eabi-gcc git stlink
```

### Windows (**TBD**)

## Hardware Requirements

- **Board:** STM32F7508-DK Discovery Board.
- **Storage:** MicroSD Card formatted to FAT32.
- **Controls:** Custom push buttons wired to the board.
- **Display:** Onboard 480x272 RGB LCD.

## Key Features

- **Hardware-Accelerated Rendering:** Uses the STM32 DMA2D (Chrom-ART Accelerator) in `DMA2D_M2M_PFC` mode. The engine renders natively in 8-bit mode (320x200), and the DMA2D hardware uses a Color Look-Up Table (CLUT) to convert the 8-bit palette to 32-bit ARGB8888 pixels on the fly, centering the image on the 480x272 LCD with zero CPU overhead.
- **Advanced Memory Management:** The STM32F7508 has only 320 KB of internal RAM, which is insufficient for Doom. This port successfully routes the massive memory footprint into the onboard 16MB MT48LC4M32B2B5-6A SDRAM via a custom linker script and MPU (Memory Protection Unit) configuration.
    - `0xC0000000`: 1MB Hardware LCD Framebuffer.
    - `0xC0100000`: Doom Zone Memory (Heap), rendering arrays, and dynamic allocations.
- **RTOS & Filesystem:** Powered by FreeRTOS for task management and FatFs for reading WAD files directly from the MicroSD card using non-blocking DMA transfers.

## Getting Started (Linux)

- Clone the repository and enter project directory

```bash
git clone <repository_url>
cd stm32_doom_port
```

- Update all submodules

```bash
git submodule update --init --recursive
```

### Bootloader

- Build Bootloader

```bash
mkdir -p build_bootloader
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=$PWD/cmake/gcc-arm-none-eabi.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S bootloader/ -B build_bootloader -G Ninja
```

- Compile Bootloader

```bash
ninja -C build_bootloader
```

- Flash `build_bootloader/stm32f7508_bootloader.hex` using STM32CubeProgrammer

### Debug Firmware

- Build Debug

```bash
mkdir -p build/Debug
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=$PWD/cmake/gcc-arm-none-eabi.cmake -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=TRUE -S . -B build/Debug -G Ninja
```

- Compile Debug Firmware

```bash
ninja doom_port -C build/Debug
```

- Flash `build/Debug/doom_port.hex` using STM32CubeProgrammer

### SD Card Setup
The Doom engine requires a game data file (`.wad`) to run.
1. Format a MicroSD card to **FAT32**.
2. Place a valid Doom WAD file (e.g., `doom1.wad` for shareware Doom) directly into the root directory of the SD card.
3. Insert the SD card into the STM32F7508-DK.

### Controls & Wiring
The port uses standard tactile buttons wired directly between specific GPIO pins and Ground (GND). No external resistors are needed, as the project initializes the pins with internal pull-ups.

For the exact wiring diagram, pinouts, and in-game key mappings, please refer to the dedicated controls documentation:
👉 **[Controls & Key Mapping](docs/controls/controls.md)**

## Technical Notes

- **D-Cache Management:** To ensure the DMA2D hardware sees the latest rendered frames, the CPU Data Cache (D-Cache) is explicitly cleaned (`SCB_CleanDCache_by_Addr`) before every hardware graphic transfer.
- **MPU Configuration:** The SDRAM region (`0xC0000000`) is configured as Normal Cacheable memory in the MPU. This prevents hardware Bus Faults when standard C library functions (like `memset`) attempt unaligned memory accesses on the heap.
- **Interrupts:** SDMMC and DMA interrupts are strictly configured to prevent RTOS starvation and enable high-speed file reading during WAD initialization.

## Acknowledgments
- Based on [doomgeneric](https://github.com/ozkl/doomgeneric) by ozkl.
- Built using STMicroelectronics STM32 HAL, FreeRTOS, and FatFs.