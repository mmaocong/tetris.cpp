#pragma once

#include <map>
#include <string>

#define HEIGHT 20
#define WIDTH 10
#define BOARD_SIZE HEIGHT *WIDTH
#define MAX 255
#define PIECE_SIZE 4

typedef std::string str;

// dense representation of the board as pixels i.e. Canvas::Key
typedef std::array<uint8_t, BOARD_SIZE> Pixels;

// sparse representation of tetrominos on the board as indices
typedef std::array<uint8_t, PIECE_SIZE> Indices;

// map board keys to strings
typedef std::map<uint8_t, str> Dict;

// holds the current piece type and position
typedef struct {
    uint8_t shape;  // piece current shape
    Indices coord;  // current position
    Indices left;   // left position
    Indices right;  // right position
    Indices down;   // down position
    Indices rotate; // rotated position
    Indices round;  // surrounding position (detect rotation collision)
} PieceContext;

typedef struct {
    Pixels &base;   // base board
    Pixels &active; // active board (base + active piece)
    uint16_t lines; // lines cleared
    uint8_t next;   // next piece type
    char pad[5];    // padding
} GameContext;

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// Pieces i.e. tetrominos
//
// I0:               I1: [0]
//     [0][1][2][3]      [1]
//                       [2]
//                       [3]
//
// L0: [0]           L1:       [3]     L2: [3][2]        L3:
//     [1]               [0][1][2]            [1]            [2][1][0]
//     [2][3]                                 [0]            [3]
//
// T0:               T1: [2]           T2:    [3]        T3:    [0]
//     [0][1][2]         [1][3]            [2][1][0]         [3][1]
//        [3]            [0]                                    [2]
//
// O0: [0][1]
//     [2][3]
//
// Z0:               Z1:    [3]
//     [0][1]            [1][2]
//        [2][3]         [0]
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
