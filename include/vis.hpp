#pragma once

#include "tetris.hpp"

namespace Vis {

    static inline GameContext InitGame() {
        // initialize the board
        Pixels *base = new Pixels;
        Pixels *active = new Pixels;
        // set the board to empty
        memset(base, Canvas::Key::NUL, sizeof(Pixels));
        memset(active, Canvas::Key::NUL, sizeof(Pixels));
        // copy the base board to the active board
        return {*base, *active, uint16_t(0), uint8_t(rand() % Piece::NUM)};
    }

    static inline uint8_t Reload(GameContext &game) {
        // copy active board to base board
        game.base = game.active;
        uint8_t idx = game.next;
        // update next piece
        game.next = uint8_t(rand() % Piece::NUM);
        return idx;
    }

    // add a piece to the board
    static inline void UpdateBoard(const PieceContext &piece,
                                   GameContext &dst) {
        // copy the base board to the active board
        dst.active = dst.base;
        // Place Tetromino on output board
        for (const auto &i : piece.coord) {
            dst.active[i] = Canvas::Key::BLK;
        }
    }

    // check if a row is full
    static inline bool CheckRowFull(const Pixels &board, const uint8_t row) {
        // check if the row is full
        return std::all_of(board.begin() + row * WIDTH,
                           board.begin() + (row + 1) * WIDTH,
                           [](const auto &i) { return i != Canvas::Key::NUL; });
    }

    // remove a row from the board
    //
    // 1. shift above rows down
    // 2. set the top row to empty
    static inline void RmRow(Pixels &board, const uint8_t row) {
        // shift by std::copy rows [0, row) to rows [1, row + 1)
        std::copy(board.begin(), board.begin() + row * WIDTH,
                  board.begin() + WIDTH);
        // set the top row to empty
        std::fill(board.begin(), board.begin() + WIDTH, Canvas::Key::NUL);
    }

    // check for full rows and remove them
    static inline uint16_t RowsExplode(Pixels &board) {
        uint16_t count = 0;
        // check for full rows
        for (uint8_t row = 0; row < HEIGHT; ++row) {
            // check if the row is full
            if (CheckRowFull(board, row)) {
                // remove the row
                RmRow(board, row);
                // increment the score
                ++count;
            }
        }
        return count;
    }

    // visualize the board
    void ToStr(const Pixels &, const Dict &, str &);

} // namespace Vis
