#pragma once

#include "const.hpp"

namespace piece {

    // holds the current piece type and position
    struct Context {
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

} // namespace piece
