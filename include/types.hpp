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
#define BOARD_SIZE HEIGHT *WIDTH
#define MAX 255
#define PIECE_SIZE 4

typedef std::chrono::seconds sec;
typedef std::chrono::steady_clock steady_clock;
typedef std::chrono::time_point<steady_clock> time_point;

// dense representation of the board as pixels i.e. Canvas::Key
typedef std::array<uint8_t, BOARD_SIZE> Pixels;

// sparse representation of tetrominos on the board as indices
typedef std::array<uint8_t, PIECE_SIZE> Indices;

// for terminal UI
typedef std::array<std::string, HEIGHT> Screen;

// map board keys to strings
typedef std::map<uint8_t, std::string> Dict;

// holds the current piece type and position
typedef struct {
    uint8_t shape;  // piece current shape
    Indices coord;  // current position
    Indices left;   // left position
    Indices right;  // right position
    Indices down;   // down position
    Indices rotate; // rotated position
    Indices round;  // surrounding position (detect rotation collision)
} PieceContext;

typedef struct {
    time_point last_fall; // last fall timestamp
    Pixels &base;         // base board
    Pixels &active;       // active board (base + active piece)
    uint16_t lines;       // lines cleared
    uint8_t next;         // next piece type
    char pad[5];          // padding
} GameContext;
