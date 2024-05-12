// ----------------------------------------------------------------------------
// types.hpp
//
// Define the types / structs used in the game.
// ----------------------------------------------------------------------------

#pragma once

#include "const.hpp"

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

    // Deliberate using max value of uint8_t to represent invalids
    static inline constexpr uint8_t kNanInd = UINT8_MAX;

    // Each piece consists of 4 cells; that's why it's also called tetromino
    static inline constexpr uint8_t kNPiece = 4;

    // static constexpr uint8_t NUM = 5;

    // sparse representation of tetrominos on the board as indices
    using Coord = std::array<uint8_t, kNPiece>;

    enum class Type : uint8_t {
        I = 0,
        L,
        T,
        Z,
        O,
    };

    enum class Shape : uint8_t {
        I0 = 0,
        I1,
        L0,
        L1,
        L2,
        L3,
        T0,
        T1,
        T2,
        T3,
        Z0,
        Z1,
        O0,
    };

    static inline const Type rand_type() {
        return static_cast<Type>(rand() % 5);
    }

    static inline constexpr Shape next_shape(const Shape &cur) {
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
            return static_cast<Shape>(static_cast<uint8_t>(cur) + 1);
        }
    }

    // Piece initial positions
    namespace Init {

        static constexpr Coord I = {3, 4, 5, 6};
        static constexpr Coord L = {4, 14, 24, 25};
        static constexpr Coord T = {4, 5, 6, 15};
        static constexpr Coord Z = {4, 5, 15, 16};
        static constexpr Coord O = {4, 5, 14, 15};

    } // namespace Init

    namespace {
        // get row ID from index
        static inline constexpr uint8_t row(const uint8_t &idx) {
            return idx / WIDTH;
        }

        // get column ID from index
        static inline constexpr uint8_t col(const uint8_t &idx) {
            return idx % WIDTH;
        }

        // index of the cell which will be occupied when moving left
        static inline constexpr uint8_t l(const uint8_t &idx) {
            if (col(idx) == 0 || idx == kNanInd) {
                return kNanInd;
            }
            return idx - 1;
        }

        // index of the cell which will be occupied when moving right
        static inline constexpr uint8_t r(const uint8_t &idx) {
            if (col(idx) == WIDTH - 1 || idx == kNanInd) {
                return kNanInd;
            }
            return idx + 1;
        }

        // index of the cell which will be occupied when moving up
        static inline constexpr uint8_t u(const uint8_t &idx) {
            if (row(idx) == 0 || idx == kNanInd) {
                return kNanInd;
            }
            return idx - WIDTH;
        }

        // index of the cell which will be occupied when moving down
        static inline constexpr uint8_t d(const uint8_t &idx) {
            if (row(idx) == HEIGHT - 1 || idx == kNanInd) {
                return kNanInd;
            }
            return idx + WIDTH;
        }

    } // namespace

    // update rotate coord based on current position and shape
    // pieces should always follow the top-down, left-right order
    static inline void update_rot(const Coord &src, const Piece::Shape &shape,
                                  Coord &dst) {
        switch (shape) {
        // from I0 to I1
        //
        // [ ][ ]          [ ][0]
        // [0][1][2][3] -> [ ][1][ ][ ]
        //    [ ][ ][ ]       [2][ ][ ]
        //    [ ][ ][ ]       [3][ ][ ]
        case Piece::Shape::I0:
            dst = {u(src[1]), src[1], d(src[1]), d(d(src[1]))};
            break;
        // from I1 to I0
        //
        // [ ][0]          [ ][ ]
        // [ ][1][ ][ ] -> [0][1][2][3]
        //    [2][ ][ ]       [ ][ ][ ]
        //    [3][ ][ ]       [ ][ ][ ]
        case Piece::Shape::I1:
            dst = {l(src[1]), src[1], r(src[1]), r(r(src[1]))};
            break;
        // from L0 to L1
        //
        // [ ][0][ ]    [ ][ ][0]
        // [ ][1][ ] -> [1][2][3]
        //    [2][3]       [ ][ ]
        case Piece::Shape::L0:
            dst = {r(src[0]), l(src[1]), src[1], r(src[1])};
            break;
        // from L1 to L2
        //
        // [ ][ ][0]    [0][1][ ]
        // [1][2][3] -> [ ][2][ ]
        // [ ][ ]       [ ][3]
        case Piece::Shape::L1:
            dst = {u(src[1]), l(src[0]), src[2], d(src[2])};
            break;
        // from L2 to L3
        //
        // [0][1]       [ ][ ]
        // [ ][2][ ] -> [0][1][2]
        // [ ][3][ ]    [3][ ][ ]
        case Piece::Shape::L2:
            dst = {l(src[2]), src[2], r(src[2]), l(src[3])};
            break;
        // from L3 to L0
        //
        //    [ ][ ]       [0][ ]
        // [0][1][2] -> [ ][1][ ]
        // [3][ ][ ]    [ ][2][3]
        case Piece::Shape::L3:
            dst = {u(src[1]), src[1], r(src[3]), d(src[2])};
            break;
        // from T0 to T1
        //
        //    [ ][ ]       [0][ ]
        // [0][1][2] -> [ ][1][2]
        // [ ][3][ ]    [ ][3][ ]
        case Piece::Shape::T0:
            dst = {u(src[1]), src[1], src[2], src[3]};
            break;
        // from T1 to T2
        //
        // [ ][0][ ]    [ ][0][ ]
        // [ ][1][2] -> [1][2][3]
        //    [3][ ]       [ ][ ]
        case Piece::Shape::T1:
            dst = {src[0], l(src[1]), src[1], src[2]};
            break;
        // from T2 to T3
        //
        // [ ][0][ ]    [ ][0][ ]
        // [1][2][3] -> [1][2][ ]
        // [ ][ ]       [ ][3]
        case Piece::Shape::T2:
            dst = {src[0], src[1], src[2], d(src[2])};
            break;
        // from T3 to T0
        //
        // [ ][0]       [ ][ ]
        // [1][2][ ] -> [0][1][2]
        // [ ][3][ ]    [ ][3][ ]
        case Piece::Shape::T3:
            dst = {src[1], src[2], r(src[2]), src[3]};
            break;
        // from Z0 to Z1
        //
        //       [ ]          [0]
        // [0][1][ ] -> [ ][1][2]
        // [ ][2][3]    [ ][3][ ]
        case Piece::Shape::Z0:
            dst = {u(r(src[1])), src[1], r(src[1]), src[2]};
            break;
        // from Z1 to Z0
        //
        //       [0][ ]          [ ][ ]
        // [ ][1][2][ ] -> [0][1][ ][ ]
        // [ ][3][ ]       [ ][2][3]
        case Piece::Shape::Z1:
            dst = {l(src[1]), src[1], src[3], r(src[3])};
            break;
        default:
            dst = src;
            break;
        }
    }

    // update indices of cells around the piece which may prevent rotation
    // (rotated cells are not included)
    //
    // TODO: currently use placeholder when no more than 5 cells are
    //       occupied
    static inline void update_ard(const Coord &src, const Piece::Shape &shape,
                                  Coord &dst) {
        uint8_t plh = src[0]; // placeholder
        switch (shape) {
        // from I0 to I1
        //
        // [ ]             [ ][0]
        // [0][1][2][3] ->    [1]
        //       [ ][ ]       [2][ ][ ]
        //       [ ]          [3][ ]
        case Piece::Shape::I0:
            dst = {u(src[0]), d(src[2]), d(src[3]), d(d(src[2]))};
            break;

        // from I1 to I0
        //
        // [ ][0]          [ ]
        //    [1]       -> [0][1][2][3]
        //    [2][ ][ ]          [ ][ ]
        //    [3][ ]             [ ]
        case Piece::Shape::I1:
            dst = {l(src[0]), r(src[2]), r(r(src[2])), r(src[3])};
            break;

        // from L0 to L1
        //
        // [ ][0]       [ ]   [0]
        //    [1]    -> [1][2][3]
        //    [2][3]
        case Piece::Shape::L0:
            dst = {l(src[0]), plh, plh, plh};
            break;

        // from L1 to L2
        //
        //       [0]    [0][1]
        // [0][1][3] ->    [2]
        // [ ]          [ ][3]
        case Piece::Shape::L1:
            dst = {d(src[0]), plh, plh, plh};
            break;

        // from L2 to L3
        //
        // [0][1]
        //    [2]    -> [X][X][X]
        //    [3][ ]    [X]   [ ]
        case Piece::Shape::L2:
            dst = {r(src[3]), plh, plh, plh};
            break;

        // from L3 to L0
        //
        //       [ ]    [X][ ]
        // [0][1][2] -> [X]
        // [3]          [X][X]
        case Piece::Shape::L3:
            dst = {u(src[2]), plh, plh, plh};
            break;

        // from T0 to T1
        //
        //       [ ]       [X][ ]
        // [0][1][2] ->    [X][X]
        // [ ][3][ ]    [ ][X][ ]
        case Piece::Shape::T0:
            dst = {u(src[2]), d(src[0]), d(src[2]), plh};
            break;

        // from T1 to T2
        //
        // [ ][0][ ]    [ ][X][ ]
        //    [1][2] -> [X][X][X]
        //    [3][ ]          [ ]
        case Piece::Shape::T1:
            dst = {l(src[0]), r(src[0]), r(src[3]), plh};
            break;

        // from T2 to T3
        //
        // [ ][0][ ]    [ ][X][ ]
        // [1][2][3] -> [X][X]
        // [ ]          [ ][X]
        case Piece::Shape::T2:
            dst = {l(src[0]), r(src[0]), d(src[1]), plh};
            break;

        // from T3 to T0
        //
        // [ ][0]       [ ]
        // [1][2]    -> [X][X][X]
        // [ ][3][ ]    [ ][X][ ]
        case Piece::Shape::T3:
            dst = {l(src[0]), l(src[3]), r(src[3]), plh};
            break;

        // from Z0 to Z1
        //
        //                    [X]
        // [0][1]    ->    [X][X]
        // [ ][2][3]    [ ][X]
        case Piece::Shape::Z0:
            dst = {d(src[0]), plh, plh, plh};
            break;

        // from Z1 to Z0
        //
        //    [3][ ]          [ ]
        // [1][2][ ] -> [X][X][ ]
        // [0]             [X][X]
        case Piece::Shape::Z1:
            dst = {r(src[3]), r(src[2]), plh, plh};
            break;

        default:
            dst = {plh, plh, plh, plh};
            break;
        }
    }

    // update left coord
    static inline void update_left(const Coord &src, Coord &dst) {
        for (uint8_t i = 0; i < kNPiece; ++i) {
            dst[i] = l(src[i]);
        }
    }

    // update right coord
    static inline void update_right(const Coord &src, Coord &dst) {
        for (uint8_t i = 0; i < kNPiece; ++i) {
            dst[i] = r(src[i]);
        }
    }

    // update down coord
    static inline void update_down(const Coord &src, Coord &dst) {
        for (uint8_t i = 0; i < kNPiece; ++i) {
            dst[i] = d(src[i]);
        }
    }

    // holds the current piece type and position
    struct Context {
        Shape shape;  // piece current shape
        Coord cur;    // current position
        Coord left;   // left position
        Coord right;  // right position
        Coord down;   // down position
        Coord rotate; // rotated position
        Coord round;  // surrounding position (detect rotation collision)

        // initialize piece context according to index
        inline void Spawn(const Type &index) {
            switch (index) {
            case Type::I:
                shape = Shape::I0;
                cur = Init::I;
                break;
            case Type::L:
                shape = Shape::L0;
                cur = Init::L;
                break;
            case Type::T:
                shape = Shape::T0;
                cur = Init::T;
                break;
            case Type::Z:
                shape = Shape::Z0;
                cur = Init::Z;
                break;
            default:
                shape = Shape::O0;
                cur = Init::O;
                break;
            }

            update_left(cur, left);
            update_right(cur, right);
            update_down(cur, down);
            update_rot(cur, shape, rotate);
            update_ard(cur, shape, round);
        }

        inline void Rotate() {
            // replace current with rotate
            cur = rotate;
            // update shape
            shape = next_shape(shape);

            // rotate go rotate
            update_rot(cur, shape, rotate);

            // update left, right, down and around
            update_left(cur, left);
            update_right(cur, right);
            update_down(cur, down);
            update_ard(cur, shape, round);
        }

        inline void Left() {
            // replace right with current
            right = cur;
            // replace current with left
            cur = left;
            // left move left
            update_left(cur, left);
            // update down, rotate and around
            update_down(cur, down);
            update_rot(cur, shape, rotate);
            update_ard(cur, shape, round);
        }

        inline void Right() {
            // replace left with current
            left = cur;
            // replace current with right
            cur = right;
            // right move right
            update_right(cur, right);
            // update down, rotate and around
            update_down(cur, down);
            update_rot(cur, shape, rotate);
            update_ard(cur, shape, round);
        }

        inline void Down() {
            // replace current with down
            cur = down;
            // down move down
            update_down(cur, down);
            // update left, right, rotate and around
            update_left(cur, left);
            update_right(cur, right);
            update_rot(cur, shape, rotate);
            update_ard(cur, shape, round);
        }
    };

} // namespace Piece
