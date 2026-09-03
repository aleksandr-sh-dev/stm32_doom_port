
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

## doomgeneric
set(DOOMGENERIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../libs/doomgeneric/doomgeneric")

set(DoomGeneric_Inc
    ${DOOMGENERIC_DIR}
)

file(GLOB DoomGeneric_Src "${CMAKE_CURRENT_SOURCE_DIR}/../../libs/doomgeneric/doomgeneric/*.c")
list(FILTER DoomGeneric_Src EXCLUDE REGEX "doomgeneric_(win|sdl|xlib|soso|sosox|allegro|emscripten|linuxvt)\\.c$")
list(FILTER DoomGeneric_Src EXCLUDE REGEX "i_allegromusic.c")
list(FILTER DoomGeneric_Src EXCLUDE REGEX "i_allegrosound.c")
list(FILTER DoomGeneric_Src EXCLUDE REGEX "i_sdlmusic.c")
list(FILTER DoomGeneric_Src EXCLUDE REGEX "i_sdlsound.c")

target_sources(${CMAKE_PROJECT_NAME} PRIVATE ${DoomGeneric_Src})

# Create libs library
add_library(libs INTERFACE)
target_include_directories(libs INTERFACE ${LWPRINTF_Inc} ${DoomGeneric_Inc})
