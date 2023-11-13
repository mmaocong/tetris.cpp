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

    // visualize the board
    void ToStr(const Pixels &, const Dict &, str &);

} // namespace Vis
