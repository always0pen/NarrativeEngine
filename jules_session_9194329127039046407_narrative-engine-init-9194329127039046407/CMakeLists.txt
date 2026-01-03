cmake_minimum_required(VERSION 3.20)
project(NarrativeEngine VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add source directory
include_directories(src)

# Find threads
find_package(Threads REQUIRED)

# Core library
add_library(core 
    src/core/board.cpp
    src/core/movegen.cpp
    src/core/position.cpp
    src/inference/factory.cpp
)

if(APPLE)
    # Add Metal frameworks and Obj-C++ sources
    find_library(METAL_LIB Metal)
    find_library(MPSGRAPH_LIB MetalPerformanceShadersGraph)
    find_library(FOUNDATION_LIB Foundation)
    
    target_sources(core PRIVATE 
        src/inference/mps_backend.mm
        src/utils/shared_memory.mm
    )
    target_link_libraries(core PRIVATE ${METAL_LIB} ${MPSGRAPH_LIB} ${FOUNDATION_LIB})
else()
    target_sources(core PRIVATE src/utils/shared_memory.cpp)
endif()

# Main executable (placeholder for now)
add_executable(narrative_engine src/engine/main.cpp)
target_link_libraries(narrative_engine PRIVATE core Threads::Threads)

# Tests
enable_testing()
add_executable(test_board tests/test_board.cpp)
target_link_libraries(test_board PRIVATE core)
add_test(NAME test_board COMMAND test_board)

add_executable(test_simd tests/test_simd.cpp)
target_link_libraries(test_simd PRIVATE core)
add_test(NAME test_simd COMMAND test_simd)

add_executable(test_tensor tests/test_tensor.cpp)
target_link_libraries(test_tensor PRIVATE core)
add_test(NAME test_tensor COMMAND test_tensor)
