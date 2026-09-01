target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_sources(app PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/log.c
${CMAKE_CURRENT_SOURCE_DIR}/lcd_log.c
)