cmake_minimum_required(VERSION 3.13)

project(dze_memory)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
include(compiler_options)

include(thirdparty/mimalloc)

add_library(dze_memory INTERFACE)
target_include_directories(dze_memory INTERFACE include)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR
    ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")

    target_compile_options(dze_memory INTERFACE -fsized-deallocation)
endif ()

target_link_libraries(dze_memory INTERFACE mimalloc-static)

add_library(dze::memory ALIAS dze_memory)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    option(${PROJECT_NAME}_static_analyzer "Enable Clang-Tidy" ON)

    if (${PROJECT_NAME}_static_analyzer)
        include(clang-tidy)
    endif ()

    option(${PROJECT_NAME}_benchmark "Enable benchmarks" ON)

    if (${PROJECT_NAME}_benchmark)
        add_subdirectory(benchmark)
    endif ()
endif ()
