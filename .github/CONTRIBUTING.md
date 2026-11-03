# Contributing to STM32F7508-DK Doom Port

Thank you for your interest in contributing! Because this is a bare-metal embedded project running on specific hardware, please follow these guidelines when submitting pull requests.

## Development Workflow
1. Fork the repository and create your feature branch from `develop` or `master`.
2. Ensure your custom code adheres to formatting standards by running `clang-format`.
3. Verify that all targets (`bootloader`, `Debug`, and `Release`) compile successfully using CMake and Ninja.

## Testing on Hardware
* Test your changes physically on an **STM32F7508-DK** discovery board.
* Verify that the MicroSD card successfully mounts `doom1.wad` via FatFS without throwing `FR_INT_ERR` or cache coherency faults.
* Check linker memory usage (`arm-none-eabi-size`) to ensure internal SRAM, DTCMRAM, and SDRAM limits are not exceeded.