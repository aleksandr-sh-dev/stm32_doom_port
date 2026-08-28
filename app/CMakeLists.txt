cmake_minimum_required(VERSION 3.22)

add_library(app STATIC)

FILE(GLOB app_sources src/*.c)
target_sources(app PRIVATE ${app_sources})

# Applications
add_subdirectory(src/log)

target_link_libraries(app PRIVATE lwprintf)
target_link_libraries(app PRIVATE core)
