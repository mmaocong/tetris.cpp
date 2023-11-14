#pragma once

#include "tetris.hpp"

namespace Move {

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
        static inline void o_coord(const Indices &src, const uint8_t &shape,
                                   Indices &dst) {
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
            // [ ][0][ ]    [ ][ ][3]
            // [ ][1][ ] -> [0][1][2]
            //    [2][3]       [ ][ ]
            case Piece::Shape::L0:
                dst = {l(src[1]), src[1], r(src[1]), r(src[0])};
                break;
            // from L1 to L2
            //
            // [ ][ ][3]    [3][2][ ]
            // [0][1][2] -> [ ][1][ ]
            // [ ][ ]       [ ][0]
            case Piece::Shape::L1:
                dst = {d(src[1]), src[1], l(src[3]), u(src[0])};
                break;
            // from L2 to L3
            //
            // [3][2]       [ ][ ]
            // [ ][1][ ] -> [2][1][0]
            // [ ][0][ ]    [3][ ][ ]
            case Piece::Shape::L2:
                dst = {r(src[1]), src[1], d(src[3]), l(src[0])};
                break;
            // from L3 to L0
            //
            //    [ ][ ]       [0][ ]
            // [2][1][0] -> [ ][1][ ]
            // [3][ ][ ]    [ ][2][3]
            case Piece::Shape::L3:
                dst = {u(src[1]), src[1], r(src[3]), d(src[0])};
                break;
            // from T0 to T1
            //
            //    [ ][ ]       [2][ ]
            // [0][1][2] -> [ ][1][3]
            // [ ][3][ ]    [ ][0][ ]
            case Piece::Shape::T0:
                dst = {src[3], src[1], u(src[1]), src[2]};
                break;
            // from T1 to T2
            //
            // [ ][2][ ]    [ ][3][ ]
            // [ ][1][3] -> [2][1][0]
            //    [0][ ]       [ ][ ]
            case Piece::Shape::T1:
                dst = {src[3], src[1], l(src[1]), src[2]};
                break;
            // from T2 to T3
            //
            // [ ][3][ ]    [ ][0][ ]
            // [2][1][0] -> [3][1][ ]
            // [ ][ ]       [ ][2]
            case Piece::Shape::T2:
                dst = {src[3], src[1], d(src[1]), src[2]};
                break;
            // from T3 to T0
            //
            // [ ][0]       [ ][ ]
            // [3][1][ ] -> [0][1][2]
            // [ ][2][ ]    [ ][3][ ]
            case Piece::Shape::T3:
                dst = {src[3], src[1], r(src[1]), src[2]};
                break;
            // from Z0 to Z1
            //
            //       [ ]          [3]
            // [0][1][ ] -> [ ][1][2]
            // [ ][2][3]    [ ][0][ ]
            case Piece::Shape::Z0:
                dst = {src[2], src[1], r(src[1]), r(u(src[1]))};
                break;
            // from Z1 to Z0
            //
            //       [3]          [ ]
            // [ ][1][2] -> [0][1][ ]
            // [ ][0][ ]    [ ][2][3]
            case Piece::Shape::Z1:
                dst = {l(src[1]), src[1], src[0], r(src[0])};
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
        static inline void a_coord(const Indices &src, const uint8_t &shape,
                                   Indices &dst) {
            uint8_t plh = src[0]; // placeholder
            switch (shape) {
            // from I0 to I1
            //
            // [ ]             [0]
            // [0][1][2][3] -> [1]
            //       [ ][ ]    [2]
            //       [ ]       [3]
            case Piece::Shape::I0:
                dst = {u(src[1]), d(src[2]), d(src[3]), d(d(src[2]))};
                break;

            // from I1 to I0
            //
            // [ ][0]
            //    [1]       -> [0][1][2][3]
            //    [2][ ][ ]
            //    [3][ ]
            case Piece::Shape::I1:
                dst = {l(src[0]), r(src[2]), r(r(src[2])), r(src[3])};
                break;

            // from L0 to L1
            //
            // [ ][0]             [3]
            //    [1]    -> [0][1][2]
            //    [2][3]
            case Piece::Shape::L0:
                dst = {l(src[1]), plh, plh, plh};
                break;

            // from L1 to L2
            //
            //       [3]    [3][2]
            // [0][1][2] ->    [1]
            // [ ]             [0]
            case Piece::Shape::L1:
                dst = {d(src[0]), plh, plh, plh};
                break;

            // from L2 to L3
            //
            // [3][2]
            //    [1]    -> [2][1][0]
            //    [0][ ]    [3]
            case Piece::Shape::L2:
                dst = {r(src[0]), plh, plh, plh};
                break;

            // from L3 to L0
            //
            //       [ ]    [0]
            // [2][1][0] -> [1]
            // [3]          [2][3]
            case Piece::Shape::L3:
                dst = {u(src[0]), plh, plh, plh};
                break;

            // from T0 to T1
            //
            //       [ ]    [2]
            // [0][1][2] -> [1][3]
            // [ ][3][ ]    [0]
            case Piece::Shape::T0:
                dst = {u(src[2]), d(src[0]), d(src[2]), plh};
                break;

            // from T1 to T2
            //
            // [ ][2][ ]       [3]
            //    [1][3] -> [2][1][0]
            //    [0][ ]
            case Piece::Shape::T1:
                dst = {l(src[2]), r(src[2]), r(src[0]), plh};
                break;

            // from T2 to T3
            //
            // [ ][3][ ]       [0]
            // [2][1][0] -> [3][1]
            // [ ]             [2]
            case Piece::Shape::T2:
                dst = {l(src[3]), r(src[3]), d(src[2]), plh};
                break;

            // from T3 to T0
            //
            // [ ][0]
            // [3][1]    -> [0][1][2]
            // [ ][2][ ]       [3]
            case Piece::Shape::T3:
                dst = {l(src[0]), d(src[3]), r(src[2]), plh};
                break;

            // from Z0 to Z1
            //
            //                 [3]
            // [0][1]    -> [1][2]
            //    [2][3]    [0]
            // from Z1 to Z0
            //
            //       [3]
            //    [1][2] -> [0][1]
            //    [0]          [2][3]
            default:
                dst = {plh, plh, plh, plh};
                break;
            }
        }

        // update left coord
        static inline void l_coord(const Indices &src, Indices &dst) {
            for (uint8_t i = 0; i < PIECE_SIZE; ++i) {
                dst[i] = l(src[i]);
            }
        }

        // update right coord
        static inline void r_coord(const Indices &src, Indices &dst) {
            for (uint8_t i = 0; i < PIECE_SIZE; ++i) {
                dst[i] = r(src[i]);
            }
        }

        // update down coord
        static inline void d_coord(const Indices &src, Indices &dst) {
            for (uint8_t i = 0; i < PIECE_SIZE; ++i) {
                dst[i] = d(src[i]);
            }
        }

    } // namespace

    // initialize piece context randomly
    inline void Init(const uint8_t &index, PieceContext &ctx) {
        switch (index) {
        case Piece::I:
            ctx.shape = Piece::Shape::I0;
            ctx.coord = Piece::Init::I;
            break;
        case Piece::L:
            ctx.shape = Piece::Shape::L0;
            ctx.coord = Piece::Init::L;
            break;
        case Piece::T:
            ctx.shape = Piece::Shape::T0;
            ctx.coord = Piece::Init::T;
            break;
        case Piece::Z:
            ctx.shape = Piece::Shape::Z0;
            ctx.coord = Piece::Init::Z;
            break;
        default:
            ctx.shape = Piece::Shape::O0;
            ctx.coord = Piece::Init::O;
            break;
        }

        l_coord(ctx.coord, ctx.left);
        r_coord(ctx.coord, ctx.right);
        d_coord(ctx.coord, ctx.down);
        o_coord(ctx.coord, ctx.shape, ctx.rotate);
        a_coord(ctx.coord, ctx.shape, ctx.round);
    }

    inline void Rotate(PieceContext &ctx) {
        // replace current with rotate
        ctx.coord = ctx.rotate;
        // update shape
        ctx.shape = Piece::Shape::next(ctx.shape);

        // rotate go rotate
        o_coord(ctx.coord, ctx.shape, ctx.rotate);

        // update left, right, down and around
        l_coord(ctx.coord, ctx.left);
        r_coord(ctx.coord, ctx.right);
        d_coord(ctx.coord, ctx.down);
        a_coord(ctx.coord, ctx.shape, ctx.round);
    }

    inline void Left(PieceContext &ctx) {
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
        // update down, rotate and around
        d_coord(ctx.coord, ctx.down);
        o_coord(ctx.coord, ctx.shape, ctx.rotate);
        a_coord(ctx.coord, ctx.shape, ctx.round);
    }

    inline void Right(PieceContext &ctx) {
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
        // update down, rotate and around
        d_coord(ctx.coord, ctx.down);
        o_coord(ctx.coord, ctx.shape, ctx.rotate);
        a_coord(ctx.coord, ctx.shape, ctx.round);
    }

    inline void Down(PieceContext &ctx) {
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
        // update left, right, rotate and around
        l_coord(ctx.coord, ctx.left);
        r_coord(ctx.coord, ctx.right);
        o_coord(ctx.coord, ctx.shape, ctx.rotate);
        a_coord(ctx.coord, ctx.shape, ctx.round);
    }

} // namespace Move
