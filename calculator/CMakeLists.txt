cmake_minimum_required(VERSION 3.15)
project(Calculator)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_executable(Calculator main.cpp Calculator.cpp Plugin.cpp)

function(add_plugin name)
    add_library(${name} SHARED ${CMAKE_CURRENT_SOURCE_DIR}/plugins/${name}.cpp)
    target_include_directories(${name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endfunction()

add_plugin(funcabs)
add_plugin(funccos)
add_plugin(funcctg)
add_plugin(funcln)
add_plugin(funclog)
add_plugin(funcsin)
add_plugin(funcsqrt)
add_plugin(functan)

if(WIN32)
    set_target_properties(Calculator PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
    )
    
    foreach(plugin IN ITEMS funcabs funccos funcctg funcln funclog funcsin funcsqrt functan)
        set_target_properties(${plugin} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endforeach()
endif()

add_custom_command(TARGET Calculator POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/plugins"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:funcabs>" "${CMAKE_BINARY_DIR}/plugins/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:funccos>" "${CMAKE_BINARY_DIR}/plugins/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:funcctg>" "${CMAKE_BINARY_DIR}/plugins/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:funcln>" "${CMAKE_BINARY_DIR}/plugins/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:funclog>" "${CMAKE_BINARY_DIR}/plugins/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:funcsin>" "${CMAKE_BINARY_DIR}/plugins/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:funcsqrt>" "${CMAKE_BINARY_DIR}/plugins/"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:functan>" "${CMAKE_BINARY_DIR}/plugins/"
    COMMENT "Creating plugins directory and copying DLLs"
)
