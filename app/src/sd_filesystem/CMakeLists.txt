target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(app PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/fatfs_platform.c
    ${CMAKE_CURRENT_SOURCE_DIR}/sd_diskio.c
    ${CMAKE_CURRENT_SOURCE_DIR}/fatfs.c
)