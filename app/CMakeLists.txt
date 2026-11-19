cmake_minimum_required(VERSION 3.22)

add_library(app STATIC)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

# Applications
add_subdirectory(src/log)
add_subdirectory(src/utils)
add_subdirectory(src/config)
add_subdirectory(src/doomgeneric)
add_subdirectory(src/sd_filesystem)
add_subdirectory(src/buttons)
add_subdirectory(src/audio)

target_link_libraries(app PRIVATE lwprintf)
target_link_libraries(app PRIVATE core)
target_link_libraries(app PRIVATE bsp)
target_link_libraries(app PRIVATE libs)
