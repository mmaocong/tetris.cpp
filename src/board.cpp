#include <algorithm>
#include <cstring>

#include "board.hpp"

namespace {
    enum class Pixel : uint8_t {
        NUL = 0, // empty
        BLK = 1, // block
    };

    inline constexpr uint8_t kNCell = 2;
    inline static constexpr std::array<const char[3], kNCell> kArrCellStr = {
        "  ", "[]"};
    inline static uint8_t rand_type() { return rand() % 5; }

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

} // namespace

// convert pixel to string for terminal display
std::string board::px2str(const uint8_t idx) { return kArrCellStr[idx]; }

// constructor
board::Context::Context()
    : last_fall(steady_clock::now()), base(*new_board()), active(*new_board()),
      lines(0), next(rand_type()) {}

uint8_t board::Context::Pop() {
    uint8_t next_type = this->next;
    // update next piece
    this->next = rand_type();
    return next_type;
}

void board::Context::UpdateBoard(const std::array<uint8_t, NBRK> &indices) {
    // copy the base board to the active board
    this->active = this->base;
    // Place Tetromino on output board
    for (const auto &i : indices) {
        this->active[i] = static_cast<uint8_t>(Pixel::BLK);
    }
}

void board::Context::RowsExplode() {
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

// collision / out of bound detection
// return false if it is a valid piece placement
bool board::collide(const brick_t &indices, const field_t &mx) {
    // out of bound
    for (const auto &i : indices) {
        if (i == IDX_NA) {
            return true;
        }
    }
    // collision detection
    for (const auto &i : indices) {
        if (mx[i] != static_cast<uint8_t>(Pixel::NUL)) {
            return true;
        }
    }
    return false;
}
