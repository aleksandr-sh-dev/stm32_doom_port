target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(app PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/doomgeneric_task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/doomgeneric_app.c
    ${CMAKE_CURRENT_SOURCE_DIR}/doomgeneric_audio.c
)