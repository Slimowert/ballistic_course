cmake_minimum_required(VERSION 3.10)
project(balistic)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "/Zi /Od /W3")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2 /DNDEBUG /Ot /Ob2 /Oi")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Main application
add_executable(app src/main.cpp)

set_target_properties(app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    OUTPUT_NAME "app"
)

target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# # Test executable
# add_executable(tests tests/main_tests.cpp)
# target_include_directories(tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# set_target_properties(tests PROPERTIES
#     RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#     OUTPUT_NAME "tests"
# )

# # Add test target
# enable_testing()
# add_test(NAME AllTests COMMAND tests)

# # Add clean target that removes build directory contents
# add_custom_target(clean-all
#     COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
#     COMMENT "Cleaning build directory: ${CMAKE_BINARY_DIR}"
# )
