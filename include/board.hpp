#pragma once

#include "const.hpp"

// Tetris board
//
// - The Tetris board is a 10x20 (width x height) grid of cells; its origin is
//   at the top-left corner -- this convention is also shared by `Piece`, which
//   is a sparse representation of the tetromino using a 4-cell array,
//   containing the indices of the filled cells.
// - Each cell can be empty or filled with a block.
// - The board is represented as a 1D array of 200 cells.
namespace board {

    // TODO:
    // - `next` may not belong here; it can be managed by the game loop, or the
    //   client
    // - add indices of lines exploded for animation from the client
    struct Context {
        time_point last_fall; // last fall timestamp
        field_t base;         // base board
        field_t active;       // active board (base + active piece)
        uint16_t lines;       // lines cleared
        uint8_t next;         // seed for the next piece
        char pad[5];

        // constructor
        Context();

        uint8_t Pop();

        void UpdateBoard(const std::array<uint8_t, NBRK> &);

        void RowsExplode();
    };

    // convert pixel to string for terminal display
    std::string px2str(const uint8_t);

    bool collide(const brick_t &, const field_t &);
} // namespace board
