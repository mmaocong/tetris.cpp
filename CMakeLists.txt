cmake_minimum_required(VERSION 3.27)
project(TermTetris VERSION 1.0.0)

# Specify output concise binary name
set(OUT_BIN_NAME "ttetris")

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify the compilers
set(CMAKE_C_COMPILER ${CC})
set(CMAKE_CXX_COMPILER ${CXX})

# Force CMake to use ld.lld, and set linker flags
set(CMAKE_LINKER "${LD}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -fuse-ld=lld")

# -----------------------------------------------------------------------------
# Main executable
# -----------------------------------------------------------------------------
# define sources and headers
set(SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core_board.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/core_piece.cpp"
)
set(HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/include/const.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/core.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/term.hpp"
)

# Create the executable
add_executable(${OUT_BIN_NAME}
    ${SOURCES}
    ${HEADERS}
)

# Set the runtime output directory to be inside the build directory
set_target_properties(${OUT_BIN_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Configure the file into the build directory
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/include/config.h.in"
    "${CMAKE_BINARY_DIR}/include/config.h"
)

target_include_directories(${OUT_BIN_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_BINARY_DIR}/include" # Ensures config.h can be found
)
