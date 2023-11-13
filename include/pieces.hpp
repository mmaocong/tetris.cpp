#pragma once

#include "tetris.hpp"
#include <iostream>
#include <map>

#define PIECE_SIZE 4
#define LSIZE 5

// Pieces i.e. tetrominos
//
// I: [0][1][2][3]
//
// L: [0]
//    [1]
//    [2][3]
//
// O: [0][1]
//    [2][3]
//
// Z: [0][1]
//       [2][3]
//
// T: [0][1][2]
//       [3]
namespace Pieces {

    // sparse representation of tetrominos on the board
    //
    // - 4 cells + 1 type
    // - 5 cells
    using Sparse = std::array<uint8_t, LSIZE>;

    namespace Types {

        static constexpr uint8_t I0 = 0;
        static constexpr uint8_t I1 = 1;
        static constexpr uint8_t L0 = 2;
        static constexpr uint8_t L1 = 3;
        static constexpr uint8_t L2 = 4;
        static constexpr uint8_t L3 = 5;
        static constexpr uint8_t O0 = 6;
        static constexpr uint8_t Z0 = 7;
        static constexpr uint8_t Z1 = 8;
        static constexpr uint8_t T0 = 9;
        static constexpr uint8_t T1 = 10;
        static constexpr uint8_t T2 = 11;
        static constexpr uint8_t T3 = 12;

    } // namespace Types

    namespace InitCoords {

        static constexpr Sparse I = {3, 4, 5, 6, Types::I0};
        static constexpr Sparse L = {4, 14, 24, 25, Types::L0};
        static constexpr Sparse O = {4, 5, 14, 15, Types::O0};
        static constexpr Sparse Z = {4, 5, 15, 16, Types::Z0};
        static constexpr Sparse T = {4, 5, 6, 15, Types::T0};

    } // namespace InitCoords

    // holds the current piece type and position
    struct Context {
        Sparse coord;  // coordinates + type
        Sparse rotate; // rotated coordinates + type
        Sparse round;
        Sparse left;
        Sparse right;
        Sparse down;
    };

    namespace {
        // get row ID from index
        static inline uint8_t row(const uint8_t &idx) { return idx / WIDTH; }

        // get column ID from index
        static inline uint8_t col(const uint8_t &idx) { return idx % WIDTH; }

        // index of the cell which will be occupied when moving left
        static inline uint8_t l(const uint8_t &idx) {
            if (col(idx) == 0) {
                return MAX;
            }
            return idx - 1;
        }

        // index of the cell which will be occupied when moving right
        static inline uint8_t r(const uint8_t &idx) {
            if (col(idx) == WIDTH - 1) {
                return MAX;
            }
            return idx + 1;
        }

        // index of the cell which will be occupied when moving up
        static inline uint8_t u(const uint8_t &idx) {
            if (row(idx) == 0) {
                return MAX;
            }
            return idx - WIDTH;
        }

        // index of the cell which will be occupied when moving down
        static inline uint8_t d(const uint8_t &idx) {
            if (row(idx) == HEIGHT - 1) {
                return MAX;
            }
            return idx + WIDTH;
        }

        // update rotate coord and type
        static inline void o_coord(const Sparse &src, Sparse &dst) {
            switch (src[LSIZE - 1]) {
            // from I0 to I1
            //
            // [ ][ ]          [ ][0]
            // [0][1][2][3] -> [ ][1][ ][ ]
            //    [ ][ ][ ]       [2][ ][ ]
            //    [ ][ ][ ]       [3][ ][ ]
            case Types::I0:
                dst = {u(src[1]), src[1], d(src[1]), d(d(src[1])), Types::I1};
                break;
            // from I1 to I0
            //
            // [ ][0]          [ ][ ]
            // [ ][1][ ][ ] -> [0][1][2][3]
            //    [2][ ][ ]       [ ][ ][ ]
            //    [3][ ][ ]       [ ][ ][ ]
            case Types::I1:
                dst = {l(src[1]), src[1], r(src[1]), r(r(src[1])), Types::I0};
                break;
            // from L0 to L1
            //
            // [ ][0][ ]    [ ][ ][3]
            // [ ][1][ ] -> [0][1][2]
            //    [2][3]       [ ][ ]
            case Types::L0:
                dst = {l(src[1]), src[1], r(src[1]), r(src[0]), Types::L1};
                break;
            // from L1 to L2
            //
            // [ ][ ][3]    [3][2][ ]
            // [0][1][2] -> [ ][1][ ]
            // [ ][ ]       [ ][0]
            case Types::L1:
                dst = {d(src[1]), src[1], l(src[3]), u(src[0]), Types::L2};
                break;
            // from L2 to L3
            //
            // [3][2]       [ ][ ]
            // [ ][1][ ] -> [2][1][0]
            // [ ][0][ ]    [3][ ][ ]
            case Types::L2:
                dst = {r(src[1]), src[1], d(src[3]), l(src[0]), Types::L3};
                break;
            // from L3 to L0
            //
            //    [ ][ ]       [0][ ]
            // [2][1][0] -> [ ][1][ ]
            // [3][ ][ ]    [ ][2][3]
            case Types::L3:
                dst = {u(src[1]), src[1], r(src[3]), d(src[0]), Types::L0};
                break;
            // from T0 to T1
            //
            //    [ ][ ]       [2][ ]
            // [0][1][2] -> [ ][1][3]
            // [ ][3][ ]    [ ][0][ ]
            case Types::T0:
                dst = {src[3], src[1], u(src[1]), src[2], Types::T1};
                break;
            // from T1 to T2
            //
            // [ ][2][ ]    [ ][3][ ]
            // [ ][1][3] -> [2][1][0]
            //    [0][ ]       [ ][ ]
            case Types::T1:
                dst = {src[3], src[1], l(src[1]), src[2], Types::T2};
                break;
            // from T2 to T3
            //
            // [ ][3][ ]    [ ][0][ ]
            // [2][1][0] -> [3][1][ ]
            // [ ][ ]       [ ][2]
            case Types::T2:
                dst = {src[3], src[1], d(src[1]), src[2], Types::T3};
                break;
            // from T3 to T0
            //
            // [ ][0]       [ ][ ]
            // [3][1][ ] -> [0][1][2]
            // [ ][2][ ]    [ ][3][ ]
            case Types::T3:
                dst = {src[3], src[1], r(src[1]), src[2], Types::T0};
                break;
            // from Z0 to Z1
            //
            //       [ ]          [3]
            // [0][1][ ] -> [ ][1][2]
            // [ ][2][3]    [ ][0][ ]
            case Types::Z0:
                dst = {src[2], src[1], r(src[1]), r(u(src[1])), Types::Z1};
                break;
            // from Z1 to Z0
            //
            //       [3]          [ ]
            // [ ][1][2] -> [0][1][ ]
            // [ ][0][ ]    [ ][2][3]
            case Types::Z1:
                dst = {l(src[1]), src[1], src[0], r(src[0]), Types::Z0};
                break;
            default:
                break;
            }
        }

        // update left coord
        static inline void l_coord(const Sparse &src, Sparse &dst) {
            for (uint8_t i = 0; i < PIECE_SIZE; ++i) {
                dst[i] = l(src[i]);
            }
            dst[LSIZE - 1] = src[LSIZE - 1];
        }

        // update right coord
        static inline void r_coord(const Sparse &src, Sparse &dst) {
            for (uint8_t i = 0; i < PIECE_SIZE; ++i) {
                dst[i] = r(src[i]);
            }
            dst[LSIZE - 1] = src[LSIZE - 1];
        }

        // update down coord
        static inline void d_coord(const Sparse &src, Sparse &dst) {
            for (uint8_t i = 0; i < PIECE_SIZE; ++i) {
                dst[i] = d(src[i]);
            }
            dst[LSIZE - 1] = src[LSIZE - 1];
        }

    } // namespace

    inline void InitContext(Context &ctx) {
        l_coord(ctx.coord, ctx.left);
        r_coord(ctx.coord, ctx.right);
        d_coord(ctx.coord, ctx.down);
        o_coord(ctx.coord, ctx.rotate);
    }

    inline void GoRotate(Context &ctx) {
        // replace current with rotate
        ctx.coord = ctx.rotate;

        // rotate go rotate
        o_coord(ctx.coord, ctx.rotate);

        // update left, right and down
        l_coord(ctx.coord, ctx.left);
        r_coord(ctx.coord, ctx.right);
        d_coord(ctx.coord, ctx.down);
    }

    inline void GoLeft(Context &ctx) {
        // stop if any left cell is MAX
        for (const auto &i : ctx.left) {
            if (i == MAX) {
                return;
            }
        }
        // replace right with current
        ctx.right = ctx.coord;
        // replace current with left
        ctx.coord = ctx.left;
        // left move left
        l_coord(ctx.coord, ctx.left);
        // update down
        d_coord(ctx.coord, ctx.down);
        // update rotate
        o_coord(ctx.coord, ctx.rotate);
    }

    inline void GoRight(Context &ctx) {
        // stop if any right cell is MAX
        for (const auto &i : ctx.right) {
            if (i == MAX) {
                return;
            }
        }
        // replace left with current
        ctx.left = ctx.coord;
        // replace current with right
        ctx.coord = ctx.right;
        // right move right
        r_coord(ctx.coord, ctx.right);
        // update down
        d_coord(ctx.coord, ctx.down);
        // update rotate
        o_coord(ctx.coord, ctx.rotate);
    }

    inline void GoDown(Context &ctx) {
        // stop if any down cell is MAX
        for (const auto &i : ctx.down) {
            if (i == MAX) {
                return;
            }
        }
        // replace current with down
        ctx.coord = ctx.down;
        // down move down
        d_coord(ctx.coord, ctx.down);
        // update left and right
        l_coord(ctx.coord, ctx.left);
        r_coord(ctx.coord, ctx.right);
        // update rotate
        o_coord(ctx.coord, ctx.rotate);
    }

    // TODO
    // indices of cells occupied by rotating a piece
    //
    //   I0:           I1:
    //
    //   [ ][ ]        [ ][0]
    //   [0][1][2][3]  [ ][1][ ][ ]
    //      [ ][ ][ ]     [2][ ][ ]
    //      [ ][ ][ ]     [3][ ][ ]
    //
    //   L0:        L1:        L2:        L3:
    //
    //   [ ][0][ ]  [ ][ ][3]  [3][2]     [ ][ ]
    //   [ ][1][ ]  [0][1][2]  [ ][1][ ]  [2][1][0]
    //      [2][3]     [ ][ ]  [ ][0][ ]  [3][ ][ ]
    //
    //   T0:        T1:        T2:        T3:
    //
    //      [ ][ ]     [2][ ]  [3][2]     [ ][ ]
    //   [0][1][2]  [ ][1][3]  [ ][1][ ]  [2][1][0]
    //   [ ][3][ ]  [ ][0][ ]  [ ][0][ ]  [3][ ][ ]
    //
    //   Z0:        Z1:
    //
    //         [ ]        [3]
    //   [0][1][ ]  [ ][1][2]
    //   [ ][2][3]  [ ][0][ ]
    static inline void a_coord(const uint8_t &type, const Sparse &coord,
                               Sparse &new_coord) {}
} // namespace Pieces
