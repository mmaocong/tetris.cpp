// ----------------------------------------------------------------------------
// types.hpp
//
// Define the types / structs used in the game.
// ----------------------------------------------------------------------------

#pragma once

#include <chrono>
#include <stdint.h>

#define HEIGHT 20
#define WIDTH 10
#define TOTAL HEIGHT *WIDTH
#define IDX_NA UINT8_MAX // invalid index
#define NBRK 4

// Sparse representation of tetrominos on the board as indices
typedef std::array<uint8_t, NBRK> brick_t;
// Dense representation of the whole board (IMP: enum class Pixel)
typedef std::array<uint8_t, TOTAL> field_t;

typedef std::chrono::seconds sec;
typedef std::chrono::steady_clock steady_clock;
typedef std::chrono::time_point<steady_clock> time_point;
