// ----------------------------------------------------------------------------
// Constants used in the game
// ----------------------------------------------------------------------------

#pragma once

#include <string>
#include <array>

#include "types.hpp"

// Pieces i.e. tetrominos
//
// I0:               I1: [0]
//     [0][1][2][3]      [1]
//                       [2]
//                       [3]
//
// L0: [0]           L1:       [0]     L2: [0][1]        L3:
//     [1]               [1][2][3]            [2]            [0][1][2]
//     [2][3]                                 [3]            [3]
//
// T0:               T1: [0]           T2:    [0]        T3:    [0]
//     [0][1][2]         [1][2]            [1][2][3]         [1][2]
//        [3]            [3]                                    [3]
//
// O0: [0][1]
//     [2][3]
//
// Z0:               Z1:    [0]
//     [0][1]            [1][2]
//        [2][3]         [3]
namespace Piece {

    static constexpr uint8_t NUM = 5;

    static constexpr uint8_t I = 0;
    static constexpr uint8_t L = 1;
    static constexpr uint8_t T = 2;
    static constexpr uint8_t Z = 3;
    static constexpr uint8_t O = 4;

    namespace Shape {

        static constexpr uint8_t I0 = 0;
        static constexpr uint8_t I1 = 1;
        static constexpr uint8_t L0 = 2;
        static constexpr uint8_t L1 = 3;
        static constexpr uint8_t L2 = 4;
        static constexpr uint8_t L3 = 5;
        static constexpr uint8_t T0 = 6;
        static constexpr uint8_t T1 = 7;
        static constexpr uint8_t T2 = 8;
        static constexpr uint8_t T3 = 9;
        static constexpr uint8_t Z0 = 10;
        static constexpr uint8_t Z1 = 11;
        static constexpr uint8_t O0 = 12;

        // get the next piece type
        static inline constexpr uint8_t next(const uint8_t &cur) {
            switch (cur) {
            case Shape::I1:
                return Shape::I0;
            case Shape::L3:
                return Shape::L0;
            case Shape::T3:
                return Shape::T0;
            case Shape::Z1:
                return Shape::Z0;
            case Shape::O0:
                return Shape::O0;
            default:
                return cur + 1;
            }
        }

    } // namespace Shape

    // Piece initial positions
    namespace Init {

        static constexpr Indices I = {3, 4, 5, 6};
        static constexpr Indices L = {4, 14, 24, 25};
        static constexpr Indices T = {4, 5, 6, 15};
        static constexpr Indices Z = {4, 5, 15, 16};
        static constexpr Indices O = {4, 5, 14, 15};

    } // namespace Init

} // namespace Piece

namespace Canvas {

    namespace Key {

        static constexpr uint8_t NUL = 0; // empty
        static constexpr uint8_t BLK = 1; // block

    } // namespace Key

    namespace Val {

        static constexpr char NUL[] = "  ";
        static constexpr char BLK[] = "[]";

    } // namespace Val

    // map from keys of Pixels to values of str
    static const Dict MAP = {
        {Key::NUL, Val::NUL},
        {Key::BLK, Val::BLK},
    };

} // namespace Canvas
