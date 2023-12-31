cmake_minimum_required(VERSION 3.27)

project(SimpleTetris VERSION 1.0.0)

# define sources and headers
set(SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/vis.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
)
set(HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/types.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/consts.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/move.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/term.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/vis.hpp"
)

# Create the executable
add_executable(SimpleTetris
    ${SOURCES}
    ${HEADERS}
)

# Set the runtime output directory to be inside the build directory
set_target_properties(SimpleTetris PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Configure the file into the build directory
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.in"
    "${CMAKE_BINARY_DIR}/include/config.h"
)

target_include_directories(SimpleTetris PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_BINARY_DIR}/include" # Ensures config.h can be found
)
