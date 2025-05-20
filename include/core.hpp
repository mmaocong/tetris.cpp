#pragma once

#include "const.hpp"

namespace core {

    // Tetris board
    //
    // - The Tetris board is a 10x20 (width x height) grid of cells; its origin
    //   is at the top-left corner -- this convention is also shared by
    //   `Piece`, which is a sparse representation of the tetromino using a
    //   4-cell array, containing the indices of the filled cells.
    // - Each cell can be empty or filled with a block.
    // - The board is represented as a 1D array of 200 cells.
    //
    // TODO:
    // - `next` may not belong here; it can be managed by the game loop, or the
    //   client
    // - add indices of lines exploded for animation from the client
    struct Board {
        time_point last_fall; // last fall timestamp
        field_t base;         // base board
        field_t active;       // active board (base + active piece)
        uint16_t lines;       // lines cleared
        uint8_t next;         // seed for the next piece
        char pad[5];

        // constructor
        Board();

        uint8_t Pop();

        void UpdateActive(const std::array<uint8_t, NBRK> &);

        void Land();
    };

    // holds the current piece type and position
    struct Piece {
        uint8_t state;  // piece current shape
        brick_t cur;    // current position
        brick_t left;   // left position
        brick_t right;  // right position
        brick_t down;   // down position
        brick_t rotate; // rotated position
        brick_t round;  // surrounding position (detect rotation collision)

        // initialize piece context according to index
        void Spawn(const uint8_t &);

        void Rotate();

        void Left();

        void Right();

        void Down();
    };

    bool collide(const brick_t &, const field_t &);

    bool step(core::Piece &, core::Board &);

    void ToString(const field_t &, std::array<std::string, HEIGHT> &);
} // namespace core
