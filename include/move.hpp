// ----------------------------------------------------------------------------
// Define the functions to move the piece.
// ----------------------------------------------------------------------------

#pragma once

#include "consts.hpp"
#include "types.hpp"

namespace Move {

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
            if (col(idx) == 0 || idx == MAX) {
                return MAX;
            }
            return idx - 1;
        }

        // index of the cell which will be occupied when moving right
        static inline constexpr uint8_t r(const uint8_t &idx) {
            if (col(idx) == WIDTH - 1 || idx == MAX) {
                return MAX;
            }
            return idx + 1;
        }

        // index of the cell which will be occupied when moving up
        static inline constexpr uint8_t u(const uint8_t &idx) {
            if (row(idx) == 0 || idx == MAX) {
                return MAX;
            }
            return idx - WIDTH;
        }

        // index of the cell which will be occupied when moving down
        static inline constexpr uint8_t d(const uint8_t &idx) {
            if (row(idx) == HEIGHT - 1 || idx == MAX) {
                return MAX;
            }
            return idx + WIDTH;
        }

        // update rotate coord based on current position and shape
        // pieces should always follow the top-down, left-right order
        static inline void update_rot(const Indices &src, const uint8_t &shape,
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
        static inline void update_ard(const Indices &src, const uint8_t &shape,
                                      Indices &dst) {
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
        static inline void update_left(const Indices &src, Indices &dst) {
            for (uint8_t i = 0; i < PIECE_SIZE; ++i) {
                dst[i] = l(src[i]);
            }
        }

        // update right coord
        static inline void update_right(const Indices &src, Indices &dst) {
            for (uint8_t i = 0; i < PIECE_SIZE; ++i) {
                dst[i] = r(src[i]);
            }
        }

        // update down coord
        static inline void update_down(const Indices &src, Indices &dst) {
            for (uint8_t i = 0; i < PIECE_SIZE; ++i) {
                dst[i] = d(src[i]);
            }
        }

    } // namespace

    // validate
    static inline bool Valid(const Indices &buffer, const Pixels &board) {
        // out of bound
        for (const auto &i : buffer) {
            if (i == MAX) {
                return false;
            }
        }
        // collision detection
        for (const auto &i : buffer) {
            if (board[i] != Canvas::Key::NUL) {
                return false;
            }
        }
        return true;
    }

    // initialize piece context according to index
    inline void Spawn(const uint8_t &index, PieceContext &ctx) {
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

        update_left(ctx.coord, ctx.left);
        update_right(ctx.coord, ctx.right);
        update_down(ctx.coord, ctx.down);
        update_rot(ctx.coord, ctx.shape, ctx.rotate);
        update_ard(ctx.coord, ctx.shape, ctx.round);
    }

    inline void Rotate(PieceContext &ctx) {
        // replace current with rotate
        ctx.coord = ctx.rotate;
        // update shape
        ctx.shape = Piece::Shape::next(ctx.shape);

        // rotate go rotate
        update_rot(ctx.coord, ctx.shape, ctx.rotate);

        // update left, right, down and around
        update_left(ctx.coord, ctx.left);
        update_right(ctx.coord, ctx.right);
        update_down(ctx.coord, ctx.down);
        update_ard(ctx.coord, ctx.shape, ctx.round);
    }

    inline void Left(PieceContext &ctx) {
        // replace right with current
        ctx.right = ctx.coord;
        // replace current with left
        ctx.coord = ctx.left;
        // left move left
        update_left(ctx.coord, ctx.left);
        // update down, rotate and around
        update_down(ctx.coord, ctx.down);
        update_rot(ctx.coord, ctx.shape, ctx.rotate);
        update_ard(ctx.coord, ctx.shape, ctx.round);
    }

    inline void Right(PieceContext &ctx) {
        // replace left with current
        ctx.left = ctx.coord;
        // replace current with right
        ctx.coord = ctx.right;
        // right move right
        update_right(ctx.coord, ctx.right);
        // update down, rotate and around
        update_down(ctx.coord, ctx.down);
        update_rot(ctx.coord, ctx.shape, ctx.rotate);
        update_ard(ctx.coord, ctx.shape, ctx.round);
    }

    inline void Down(PieceContext &ctx) {
        // replace current with down
        ctx.coord = ctx.down;
        // down move down
        update_down(ctx.coord, ctx.down);
        // update left, right, rotate and around
        update_left(ctx.coord, ctx.left);
        update_right(ctx.coord, ctx.right);
        update_rot(ctx.coord, ctx.shape, ctx.rotate);
        update_ard(ctx.coord, ctx.shape, ctx.round);
    }

} // namespace Move
