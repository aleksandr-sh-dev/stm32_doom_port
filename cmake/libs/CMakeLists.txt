
## Lwprintf

# Set custom Opts file
set(LWPRINTF_OPTS_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../../core/include/lwprintf_opts.h")

# Set includes
set(LWPRINTF_Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/lwprintf/lwprintf/src/include/system
)

# Set sources
set(LWPRINTF_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../libs/lwprintf/lwprintf/src/system/lwprintf_sys_cmsis_os.c
)

# Lwprintf
include(${CMAKE_CURRENT_SOURCE_DIR}/../../libs/lwprintf/lwprintf/library.cmake)

# Link core library to lwprintf
target_link_libraries(lwprintf PRIVATE core)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${LWPRINTF_Src})

# Create libs library
add_library(libs INTERFACE)
target_include_directories(libs INTERFACE ${LWPRINTF_Inc})
