// ----------------------------------------------------------------------------
// types.hpp
//
// Define the types / structs used in the game.
// ----------------------------------------------------------------------------

#pragma once

#include <chrono>
#include <map>
#include <stdint.h>

#define HEIGHT 20
#define WIDTH 10

typedef std::chrono::seconds sec;
typedef std::chrono::steady_clock steady_clock;
typedef std::chrono::time_point<steady_clock> time_point;
