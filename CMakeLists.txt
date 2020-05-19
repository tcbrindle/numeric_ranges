
cmake_minimum_required(VERSION 3.8)

project(numeric_ranges CXX)

set(CMAKE_CXX_EXTENSIONS Off)

add_library(numeric_ranges INTERFACE)

target_include_directories(numeric_ranges INTERFACE include/)
target_sources(numeric_ranges INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include/numeric_ranges.hpp)

option(USE_NANORANGE "Use NanoRange rather than std ranges")

if (USE_NANORANGE)
    target_compile_definitions(numeric_ranges INTERFACE TCB_NUMERIC_RANGES_USE_NANORANGE)
    target_include_directories(numeric_ranges INTERFACE extern/)
    target_compile_features(numeric_ranges INTERFACE cxx_std_17)
else()
    target_compile_features(numeric_ranges INTERFACE cxx_std_20)
endif()

add_subdirectory(test/)