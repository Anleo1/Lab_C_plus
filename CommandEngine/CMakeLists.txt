cmake_minimum_required(VERSION 3.10)
project(CommandEngine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0601)
    add_definitions(-DNOMINMAX)
endif()

# Убираем -Werror и уменьшаем строгость предупреждений
if(MSVC)
    add_compile_options(/W4 /EHsc)  # Убрали /WX (трактовать предупреждения как ошибки)
else()
    add_compile_options(-Wall -Wextra -pedantic)  # Убрали -Werror
endif()

add_executable(CommandEngine
    CommandEngine.cpp
    TestFunctions.cpp
)

set_source_files_properties(
    CommandEngine.cpp
    TestFunctions.cpp
    PROPERTIES LANGUAGE CXX
)

target_include_directories(CommandEngine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    target_compile_definitions(CommandEngine PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

# Настройка для отладки
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(MSVC)
        target_compile_options(CommandEngine PRIVATE /Zi /Od)
    else()
        target_compile_options(CommandEngine PRIVATE -g -O0)
    endif()
else()
    if(MSVC)
        target_compile_options(CommandEngine PRIVATE /O2)
    else()
        target_compile_options(CommandEngine PRIVATE -O3)
    endif()
endif()