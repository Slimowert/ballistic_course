cmake_minimum_required(VERSION 3.10)
project(balistic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(EIGEN3_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/lib/eigen)

add_executable(app src/main.cpp)
target_include_directories(app PRIVATE ${EIGEN3_INCLUDE_DIR})

set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME "app"
)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
