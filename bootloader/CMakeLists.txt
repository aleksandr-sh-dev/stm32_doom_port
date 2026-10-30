cmake_minimum_required(VERSION 3.22)

# Define this as a completely separate project
project(stm32f7508_bootloader)
enable_language(C ASM)

# Add the bootloader executable
add_executable(${CMAKE_PROJECT_NAME})

# Include core and BSP from the parent directory
# The second argument specifies the binary directory name inside build_bootloader/
add_subdirectory(../cmake/core core)
add_subdirectory(../board bsp)

# Find bootloader sources (ExtMem_Boot template files)
file(GLOB BOOTLOADER_SOURCES 
    "Src/*.c" 
    "startup_stm32f750xx.s"
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE 
    ${BOOTLOADER_SOURCES}
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Inc
)

# Link core and bsp libraries
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE 
    core 
    bsp
)

# Force the Internal Flash linker script for the bootloader
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -T${CMAKE_CURRENT_SOURCE_DIR}/STM32F750xx_FLASH.ld
)

# Generate HEX and BIN
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${CMAKE_PROJECT_NAME}> ${CMAKE_PROJECT_NAME}.bin
)
