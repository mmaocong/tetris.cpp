#pragma once

#include "const.hpp"
#include <algorithm>
#include <cstring>

// Tetris board
//
// - The Tetris board is a 10x20 (width x height) grid of cells; its origin is
//   at the top-left corner -- this convention is also shared by `Piece`, which
//   is a sparse representation of the tetromino using a 4-cell array,
//   containing the indices of the filled cells.
// - Each cell can be empty or filled with a block.
// - The board is represented as a 1D array of 200 cells.
namespace Board {

    namespace {
        enum class Pixel : uint8_t {
            NUL = 0, // empty
            BLK = 1, // block
        };

        inline constexpr uint8_t kNCell = 2;
        inline static constexpr std::array<const char[3], kNCell> kArrCellStr =
            {"  ", "[]"};
        inline static uint8_t rand_type() { return rand() % 5; }
    } // namespace

    // convert pixel to string for terminal display
    static inline const std::string px2str(uint8_t idx) {
        return kArrCellStr[idx];
    }

    // Initialize a `Pixels` array in heap memory with all 0s
    static inline field_t *new_board() {
        field_t *pixels = new field_t;
        memset(pixels, 0, TOTAL);
        return pixels;
    }

    // check if a row is full
    static inline bool check_row(const field_t &board, const uint8_t row) {
        // check if the row is full
        return std::all_of(board.begin() + row * WIDTH,
                           board.begin() + (row + 1) * WIDTH,
                           [](const auto &i) {
                               return i != static_cast<uint8_t>(Pixel::NUL);
                           });
    }

    // remove a row from the board
    //
    // 1. shift above rows down
    // 2. set the top row to empty
    static inline void rm_row(field_t &board, const uint8_t row) {
        // shift by std::copy rows [0, row) to rows [1, row + 1)
        std::copy(board.begin(), board.begin() + row * WIDTH,
                  board.begin() + WIDTH);
        // set the top row to empty
        std::fill(board.begin(), board.begin() + WIDTH,
                  static_cast<uint8_t>(Pixel::NUL));
    }

    // TODO:
    // - `next` may not belong here, or even `Piece`; it can be managed by the
    //   game loop, or the client
    // - add indices of lines exploded for animation from the client
    struct Context {
        time_point last_fall; // last fall timestamp
        field_t &base;        // base board
        field_t &active;      // active board (base + active piece)
        uint16_t lines;       // lines cleared
        uint8_t next; // next piece type (IMP: enum class Type in piece.hpp)
        char pad[5];

        // constructor
        Context()
            : last_fall(steady_clock::now()), base(*new_board()),
              active(*new_board()), lines(0), next(rand_type()) {}

        uint8_t Pop() {
            uint8_t next_type = this->next;
            // update next piece
            this->next = rand_type();
            return next_type;
        }

        void UpdateBoard(const std::array<uint8_t, NBRK> &indices) {
            // copy the base board to the active board
            this->active = this->base;
            // Place Tetromino on output board
            for (const auto &i : indices) {
                this->active[i] = static_cast<uint8_t>(Pixel::BLK);
            }
        }

        void RowsExplode() {
            // check for full rows
            for (uint8_t row = 0; row < HEIGHT; ++row) {
                // check if the row is full
                if (check_row(this->active, row)) {
                    // remove the row
                    rm_row(this->active, row);
                    // increment the score
                    ++this->lines;
                }
            }
            // update the base board
            this->base = this->active;
        }
    };

} // namespace Board
