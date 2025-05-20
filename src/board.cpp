#include <algorithm>
#include <cstring>
#include <random>

#include "board.hpp"

namespace {
    enum class Pixel : uint8_t {
        NUL = 0, // empty
        BLK = 1, // block
    };

    inline constexpr uint8_t kNCell = 2;
    inline static constexpr std::array<const char[3], kNCell> kArrCellStr = {
        "  ", "[]"};

    // Generate a random integer within uint8_t range
    static inline uint8_t rand_int() {
        static thread_local std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<uint8_t> dist(0, UINT8_MAX);
        return dist(rng);
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
    : last_fall(steady_clock::now()), lines(0), next(rand_int()) {
    std::fill(base.begin(), base.end(), static_cast<uint8_t>(Pixel::NUL));
    std::fill(active.begin(), active.end(), static_cast<uint8_t>(Pixel::NUL));
}

uint8_t board::Context::Pop() {
    uint8_t next_type = this->next;
    // update next piece
    this->next = rand_int();
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
        // this also checks for IDX_NA
        if (i >= TOTAL) {
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
