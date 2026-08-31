cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)
# Macros
set(MX_Defines_Syms 
	USE_HAL_DRIVER 
	STM32F750xx 
	STM32_THREAD_SAFE_STRATEGY=4
    $<$<CONFIG:Debug>:DEBUG>
)

# Include paths
set(MX_Include_Dirs
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/CMSIS_RTOS_V2
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/CMSIS/Device/ST/STM32F7xx/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FatFs/src
)

#  Application sources
set(MX_Application_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../app/main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/freertos.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/crc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/dcmi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/dma2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/eth.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/fmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/ltdc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/quadspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/rtc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/sai.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/sdmmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/spdifrx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/usart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/usb_otg.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/stm32f7xx_it.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/stm32f7xx_hal_msp.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/stm32f7xx_hal_timebase_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/sysmem.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/syscalls.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../core/source/system_stm32f7xx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../startup_stm32f750xx.s
)

# STM32 HAL/LL Drivers
set(STM32_Drivers_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_tim.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_tim_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_cortex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_crc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_crc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_rcc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_rcc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_flash.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_flash_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_gpio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_dma.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_dma_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_pwr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_pwr_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_i2c.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_i2c_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_exti.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_dcmi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_dcmi_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_dma2d.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_eth.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_ll_fmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_nor.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_sram.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_nand.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_sdram.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_ltdc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_ltdc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_dsi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_qspi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_rtc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_rtc_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_sai.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_sai_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_ll_sdmmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_sd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_mmc.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_spdifrx.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_spi.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_spi_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_uart.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_uart_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_pcd.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_hal_pcd_ex.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../drivers/stm32f7xx-hal-driver/Src/stm32f7xx_ll_usb.c
)

set(FreeRTOS_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/croutine.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/event_groups.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/list.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/queue.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/stream_buffer.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/tasks.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/timers.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/portable/MemMang/heap_4.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/port.c
)

set(FatFs_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FatFs/src/diskio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FatFs/src/ff_gen_drv.c
    ${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/FatFs/src/ff.c
)

# Link directories setup
set(MX_LINK_DIRS

)
# Project static libraries
set(MX_LINK_LIBS 
    STM32_Drivers
    ${TOOLCHAIN_LINK_LIBRARIES}
    FreeRTOS
    FatFs
)
# Interface library for includes and symbols
add_library(core INTERFACE)
target_include_directories(core INTERFACE ${MX_Include_Dirs})
target_compile_definitions(core INTERFACE ${MX_Defines_Syms})

# Create STM32_Drivers static library
add_library(STM32_Drivers OBJECT)
target_sources(STM32_Drivers PRIVATE ${STM32_Drivers_Src})
target_link_libraries(STM32_Drivers PUBLIC core)

# Create FatFs static library
add_library(FatFs OBJECT)
target_sources(FatFs PRIVATE ${FatFs_Src})
target_link_libraries(FatFs PUBLIC core)

# Create FreeRTOS static library
add_library(FreeRTOS OBJECT)
target_sources(FreeRTOS PRIVATE ${FreeRTOS_Src})
target_link_libraries(FreeRTOS PUBLIC core)

if(CMAKE_PROJECT_NAME STREQUAL "doom_port")
    # Building the Main Application
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${MX_Application_Src})
    target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${MX_LINK_DIRS})
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${MX_LINK_LIBS})
else()
    # Building the Bootloader
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE STM32_Drivers)
endif()

# Add the map file to the list of files to be removed with 'clean' target
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES ADDITIONAL_CLEAN_FILES ${CMAKE_PROJECT_NAME}.map)

# Validate that code is compatible with C standard
if((CMAKE_C_STANDARD EQUAL 90) OR (CMAKE_C_STANDARD EQUAL 99))
    message(ERROR "Generated code requires C11 or higher")
endif()
