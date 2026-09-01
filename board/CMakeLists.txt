cmake_minimum_required(VERSION 3.22)

add_library(bsp STATIC)

set(BSP_SUBMODULES
    bsp/
    bsp/STM32F7508/Common
    bsp/STM32F7508/Components
    bsp/STM32F7508/stm32f7508-dk-bsp
    Utilities/
    Utilities/Fonts
)

foreach(submodule IN LISTS BSP_SUBMODULES)
    file(GLOB submodule_sources CONFIGURE_DEPENDS "${submodule}/*.c")
    
    # Add the found sources to our target
    if(submodule_sources)
        target_sources(bsp PRIVATE ${submodule_sources})
    endif()

    # Add the submodule folder as an include directory.
    target_include_directories(bsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/${submodule})
endforeach()

file(GLOB local_sources CONFIGURE_DEPENDS "*.c")
if(local_sources)
    target_sources(bsp PRIVATE ${local_sources})
endif()
target_include_directories(bsp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(bsp PRIVATE core)
