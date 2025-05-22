#include <tuple>

#include "core.hpp"

// Pieces i.e. tetrominos
//
// O0: [0][1]
//     [2][3]
//
// I0:               I1: [0]
//     [0][1][2][3]      [1]
//                       [2]
//                       [3]
//
// T0:               T1: [0]           T2:    [0]        T3:    [0]
//     [0][1][2]         [1][2]            [1][2][3]         [1][2]
//        [3]            [3]                                    [3]
//
// Z0:               Z1:    [0]
//     [0][1]            [1][2]
//        [2][3]         [3]
//
// S0:               S1: [0]
//        [0][1]         [1][2]
//     [2][3]               [3]
//
// L0: [0]           L1:       [0]     L2: [0][1]        L3:
//     [1]               [1][2][3]            [2]            [0][1][2]
//     [2][3]                                 [3]            [3]
//
// J0:    [0]        L1:               L2: [0][1]        L3:
//        [1]            [0][1][2]         [2]               [0]
//     [2][3]                  [3]         [3]               [1][2][3]
namespace {

    // Number of initial states, and we happen to give one for each piece type
    static constexpr uint8_t kNStateInit = 7;
    // Number of piece orientation states
    static constexpr uint8_t kNState = 19;

    // Rotating Orientation
    enum class State : uint8_t {
        O0 = 0,
        I0,
        I1,
        T0,
        T1,
        T2,
        T3,
        Z0,
        Z1,
        S0,
        S1,
        L0,
        L1,
        L2,
        L3,
        J0,
        J1,
        J2,
        J3,
    };

    // Initial states of the pieces
    //
    // NOTE: pieces should always follow the top-down, left-right order, which
    // is the convention that rotation / around functions follow, otherwise
    // the piece will be misplaced after rotation
    static constexpr std::array<std::pair<uint8_t, brick_t>, kNStateInit>
        kMapBrickInit = {
            std::make_pair(static_cast<uint8_t>(State::O0),
                           brick_t{4, 5, 14, 15}),
            std::make_pair(static_cast<uint8_t>(State::I0),
                           brick_t{3, 4, 5, 6}),
            std::make_pair(static_cast<uint8_t>(State::T0),
                           brick_t{4, 5, 6, 15}),
            std::make_pair(static_cast<uint8_t>(State::Z0),
                           brick_t{4, 5, 15, 16}),
            std::make_pair(static_cast<uint8_t>(State::S0),
                           brick_t{5, 6, 14, 15}),
            std::make_pair(static_cast<uint8_t>(State::L0),
                           brick_t{4, 14, 24, 25}),
            std::make_pair(static_cast<uint8_t>(State::J0),
                           brick_t{5, 15, 24, 25}),
    };

    static constexpr std::array<char, kNStateInit> kArrShapeInit = {
        'O', 'I', 'T', 'Z', 'S', 'L', 'J',
    };

    static constexpr std::array<uint8_t, kNState> kMapState2Next = {
        static_cast<uint8_t>(State::O0), // trivial
        static_cast<uint8_t>(State::I1), // I0 -> I1
        static_cast<uint8_t>(State::I0), // I1 -> I0
        static_cast<uint8_t>(State::T1), // T0 -> T1
        static_cast<uint8_t>(State::T2), // T1 -> T2
        static_cast<uint8_t>(State::T3), // T2 -> T3
        static_cast<uint8_t>(State::T0), // T3 -> T0
        static_cast<uint8_t>(State::Z1), // Z0 -> Z1
        static_cast<uint8_t>(State::Z0), // Z1 -> Z0
        static_cast<uint8_t>(State::S1), // S0 -> S1
        static_cast<uint8_t>(State::S0), // S1 -> S0
        static_cast<uint8_t>(State::L1), // L0 -> L1
        static_cast<uint8_t>(State::L2), // L1 -> L2
        static_cast<uint8_t>(State::L3), // L2 -> L3
        static_cast<uint8_t>(State::L0), // L3 -> L0
        static_cast<uint8_t>(State::J1), // J0 -> J1
        static_cast<uint8_t>(State::J2), // J1 -> J2
        static_cast<uint8_t>(State::J3), // J2 -> J3
        static_cast<uint8_t>(State::J0), // J3 -> J0
    };

    static inline constexpr uint8_t next_state(const uint8_t state) {
        if (state >= kNState)
            throw std::out_of_range("Invalid state");
        return kMapState2Next[state];
    }

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
        if (col(idx) == 0 || idx == IDX_NA) {
            return IDX_NA;
        }
        return idx - 1;
    }

    // index of the cell which will be occupied when moving right
    static inline constexpr uint8_t r(const uint8_t &idx) {
        if (col(idx) == WIDTH - 1 || idx == IDX_NA) {
            return IDX_NA;
        }
        return idx + 1;
    }

    // index of the cell which will be occupied when moving up
    static inline constexpr uint8_t u(const uint8_t &idx) {
        if (row(idx) == 0 || idx == IDX_NA) {
            return IDX_NA;
        }
        return idx - WIDTH;
    }

    // index of the cell which will be occupied when moving down
    static inline constexpr uint8_t d(const uint8_t &idx) {
        if (row(idx) == HEIGHT - 1 || idx == IDX_NA) {
            return IDX_NA;
        }
        return idx + WIDTH;
    }

    static const std::array<brick_t (*)(const brick_t &), kNState>
        kMapState2FnRotate = {
            // O0: -> O0 (trivial)
            [](const brick_t &src) { return src; },
            // I0: -> I1
            //
            // [X][ ]          [X][0]
            // [0][1][2][3] -> [ ][1][ ][ ]
            //    [ ][ ][ ]       [2][ ][ ]
            //    [ ][ ][ ]       [3][ ][ ]
            [](const brick_t &src) {
                return brick_t{u(src[1]), src[1], d(src[1]), d(d(src[1]))};
            },
            // I1: -> I0
            //
            // [X][0]          [X][ ]
            // [ ][1][ ][ ] -> [0][1][2][3]
            //    [2][ ][ ]       [ ][ ][ ]
            //    [3][ ][ ]       [ ][ ][ ]
            [](const brick_t &src) {
                return brick_t{l(src[1]), src[1], r(src[1]), r(r(src[1]))};
            },
            // T0: -> T1
            //
            //    [ ][ ]       [0][ ]
            // [0][1][2] -> [ ][1][2]
            // [ ][3][ ]    [ ][3][ ]
            [](const brick_t &src) {
                return brick_t{u(src[1]), src[1], src[2], src[3]};
            },
            // T1: -> T2
            //
            // [ ][0][ ]    [ ][0][ ]
            // [ ][1][2] -> [1][2][3]
            //    [3][ ]       [ ][ ]
            [](const brick_t &src) {
                return brick_t{src[0], l(src[1]), src[1], src[2]};
            },
            // T2: -> T3
            //
            // [ ][0][ ]    [ ][0][ ]
            // [1][2][3] -> [1][2][ ]
            // [ ][ ]       [ ][3]
            [](const brick_t &src) {
                return brick_t{src[0], src[1], src[2], d(src[2])};
            },
            // T3: -> T0
            //
            // [ ][0]       [ ][ ]
            // [1][2][ ] -> [0][1][2]
            // [ ][3][ ]    [ ][3][ ]
            [](const brick_t &src) {
                return brick_t{src[1], src[2], r(src[2]), src[3]};
            },
            // Z0: -> Z1
            //
            //       [ ]          [0]
            // [0][1][ ] -> [ ][1][2]
            // [ ][2][3]    [ ][3][ ]
            [](const brick_t &src) {
                return brick_t{u(r(src[1])), src[1], r(src[1]), src[2]};
            },
            // Z1: -> Z0
            //
            //       [0][ ]          [ ][ ]
            // [ ][1][2][ ] -> [0][1][ ][ ]
            // [ ][3][ ]       [ ][2][3]
            [](const brick_t &src) {
                return brick_t{l(src[1]), src[1], src[3], r(src[3])};
            },
            // S0: -> S1
            //
            //    [ ]          [0]
            // [ ][0][1] -> [ ][1][2]
            // [2][3][ ]    [ ][ ][3]
            [](const brick_t &src) {
                return brick_t{u(src[0]), src[0], src[1], r(src[3])};
            },
            // S1: -> S0
            //
            // [ ][0][ ]
            // [ ][1][2] -> [ ][0][1]
            // [ ][ ][3]    [2][3][ ]
            [](const brick_t &src) {
                return brick_t{src[1], src[2], l(d(src[1])), d(src[1])};
            },
            // L0: -> L1
            //
            // [ ][0][ ]    [ ][ ][0]
            // [ ][1][ ] -> [1][2][3]
            //    [2][3]       [ ][ ]
            [](const brick_t &src) {
                return brick_t{r(src[0]), l(src[1]), src[1], r(src[1])};
            },
            // L1: -> L2
            //
            // [ ][ ][0]    [0][1][ ]
            // [1][2][3] -> [ ][2][ ]
            // [ ][ ]       [ ][3]
            [](const brick_t &src) {
                return brick_t{u(src[1]), l(src[0]), src[2], d(src[2])};
            },
            // L2: -> L3
            //
            // [0][1]       [ ][ ]
            // [ ][2][ ] -> [0][1][2]
            // [ ][3][ ]    [3][ ][ ]
            [](const brick_t &src) {
                return brick_t{l(src[2]), src[2], r(src[2]), l(src[3])};
            },
            // L3: -> L0
            //
            //    [ ][ ]       [0][ ]
            // [0][1][2] -> [ ][1][ ]
            // [3][ ][ ]    [ ][2][3]
            [](const brick_t &src) {
                return brick_t{u(src[1]), src[1], r(src[3]), d(src[2])};
            },
            // J0: -> J1
            //
            // [ ][0][ ]    [ ][ ][ ]
            // [ ][1][ ] -> [0][1][2]
            // [2][3][ ]       [ ][3]
            [](const brick_t &src) {
                return brick_t{l(src[1]), src[1], r(src[1]), r(src[3])};
            },
            // J1: -> J2
            //
            // [ ][ ][ ]    [ ][0][1]
            // [0][1][2] -> [ ][2][ ]
            //    [ ][3]       [3][ ]
            [](const brick_t &src) {
                return brick_t{u(src[1]), u(src[2]), src[1], l(src[3])};
            },
            // J2: -> J3
            //
            // [ ][0][1]    [0][ ][ ]
            // [ ][2][ ] -> [1][2][3]
            //    [3][ ]       [ ][ ]
            [](const brick_t &src) {
                return brick_t{l(src[0]), l(src[2]), src[2], r(src[2])};
            },
            // J3: -> J0
            //
            // [0][ ][ ]    [ ][0][ ]
            // [1][2][3] -> [ ][1][ ]
            //    [ ][ ]    [2][3][ ]
            [](const brick_t &src) {
                return brick_t{r(src[0]), src[2], d(src[1]), d(src[2])};
            },
    };

    // NOTE: FSM
    // update rotate coord based on current position and orientation,
    static inline void update_rot(const brick_t &src, const uint8_t state,
                                  brick_t &dst) {
        if (state >= kNState)
            return;
        dst = kMapState2FnRotate[state](src);
    }

    // TODO: currently use placeholder (`src[0]`) when no more than 5 cells
    // are occupied
    static const std::array<brick_t (*)(const brick_t &), kNState>
        kMapState2FnAround = {
            // O0: -> O0 (trivial)
            [](const brick_t &src) { return src; },
            // I0: -> I1
            //
            // [ ]             [ ][0]
            // [0][1][2][3] ->    [1]
            //       [ ][ ]       [2][ ][ ]
            //       [ ]          [3][ ]
            [](const brick_t &src) {
                return brick_t{u(src[0]), d(src[2]), d(src[3]), d(d(src[2]))};
            },
            // I1: -> I0
            //
            // [ ][0]          [ ]
            //    [1]       -> [0][1][2][3]
            //    [2][ ][ ]          [ ][ ]
            //    [3][ ]             [ ]
            [](const brick_t &src) {
                return brick_t{l(src[0]), r(src[2]), r(r(src[2])), r(src[3])};
            },
            // T0: -> T1
            //
            //       [ ]       [X][ ]
            // [0][1][2] ->    [X][X]
            // [ ][3][ ]    [ ][X][ ]
            [](const brick_t &src) {
                return brick_t{u(src[2]), d(src[0]), d(src[2]), src[0]};
            },
            // T1: -> T2
            //
            // [ ][0][ ]    [ ][X][ ]
            //    [1][2] -> [X][X][X]
            //    [3][ ]          [ ]
            [](const brick_t &src) {
                return brick_t{l(src[0]), r(src[0]), r(src[3]), src[0]};
            },
            // T2: -> T3
            //
            // [ ][0][ ]    [ ][X][ ]
            // [1][2][3] -> [X][X]
            // [ ]          [ ][X]
            [](const brick_t &src) {
                return brick_t{l(src[0]), r(src[0]), d(src[1]), src[0]};
            },
            // T3: -> T0
            //
            // [ ][0]       [ ]
            // [1][2]    -> [X][X][X]
            // [ ][3][ ]    [ ][X][ ]
            [](const brick_t &src) {
                return brick_t{l(src[0]), l(src[3]), r(src[3]), src[0]};
            },
            // Z0: -> Z1 (anti-clockwise)
            //
            //       [ ]          [X]
            // [0][1][ ] ->    [X][X]
            // ( )[2][3]    ( )[X]
            [](const brick_t &src) {
                return brick_t{d(src[0]), src[0], src[0], src[0]};
            },
            // Z1: -> Z0 (clockwise)
            //
            //    [3]( )          ( )
            // [1][2]( ) -> [X][X]( )
            // [0][ ]       [ ][X][X]
            [](const brick_t &src) {
                return brick_t{r(src[3]), r(src[2]), src[0], src[0]};
            },
            // S0: -> S1 (anti-clockwise)
            //
            //    [ ]( )       [X]
            //    [0][1] ->    [X][X]
            // [2][3][ ]          [X]
            // ( )( )
            [](const brick_t &src) {
                return brick_t{u(src[1]), d(src[2]), d(src[3]), src[0]};
            },
            // S1: -> S0 (clockwise)
            //
            // [0]( )( )
            // [1][2][ ] ->    [X][X]
            // [ ][3]       [X][X]
            [](const brick_t &src) {
                return brick_t{r(src[0]), r(r(src[0])), src[0], src[0]};
            },
            // L0: -> L1
            //
            // [ ][0]       [ ]   [0]
            //    [1]    -> [1][2][3]
            //    [2][3]
            [](const brick_t &src) {
                return brick_t{l(src[0]), src[0], src[0], src[0]};
            },
            // L1: -> L2
            //
            //       [0]    [0][1]
            // [0][1][3] ->    [2]
            // [ ]          [ ][3]
            [](const brick_t &src) {
                return brick_t{d(src[0]), src[0], src[0], src[0]};
            },
            // L2: -> L3
            //
            // [0][1]
            //    [2]    -> [X][X][X]
            //    [3][ ]    [X]   [ ]
            [](const brick_t &src) {
                return brick_t{r(src[3]), src[0], src[0], src[0]};
            },
            // L3: -> L0
            //
            //       [ ]    [X][ ]
            // [0][1][2] -> [X]
            // [3]          [X][X]
            [](const brick_t &src) {
                return brick_t{u(src[2]), src[0], src[0], src[0]};
            },
            // J0: -> J1
            //
            // ( )[0]       ( )
            // [ ][1][ ] -> [X][X][X]
            // [3][2][ ]          [X]
            [](const brick_t &src) {
                return brick_t{l(src[0]), src[0], src[0], src[0]};
            },
            // J1: -> J2
            //
            //    [ ][ ]       [X][X]
            // [0][1][2] ->    [X]
            // ( )[ ][3]    ( )[X]
            [](const brick_t &src) {
                return brick_t{d(src[0]), src[0], src[0], src[0]};
            },
            // J2: -> J3
            //
            // [ ][0][1]    [X]
            // [ ][2][ ] -> [X][X][X]
            //    [3]( )          ( )
            [](const brick_t &src) {
                return brick_t{r(src[3]), src[0], src[0], src[0]};
            },
            // J3: -> J0
            //
            // [0][ ]( )       [X]( )
            // [1][2][3] ->    [X]
            // [ ][ ]       [X][X]
            [](const brick_t &src) {
                return brick_t{u(src[3]), src[0], src[0], src[0]};
            },

    };

    // NOTE: FSM
    // update indices of cells around the piece which may prevent rotation
    // (rotated cells are not included)
    static inline void update_ard(const brick_t &src, const uint8_t state,
                                  brick_t &dst) {

        if (state >= kNState)
            return;
        dst = kMapState2FnAround[state](src);
    }

    // update left coord
    static inline void update_left(const brick_t &src, brick_t &dst) {
        for (uint8_t i = 0; i < NBRK; ++i) {
            dst[i] = l(src[i]);
        }
    }

    // update right coord
    static inline void update_right(const brick_t &src, brick_t &dst) {
        for (uint8_t i = 0; i < NBRK; ++i) {
            dst[i] = r(src[i]);
        }
    }

    // update down coord
    static inline void update_down(const brick_t &src, brick_t &dst) {
        for (uint8_t i = 0; i < NBRK; ++i) {
            dst[i] = d(src[i]);
        }
    }

} // namespace

// initialize piece context according to the seed
void core::Piece::Spawn(const uint8_t &seed) {
    const uint8_t idx = seed % kNStateInit;
    std::tie(state, cur) = kMapBrickInit.at(idx);

    update_left(cur, left);
    update_right(cur, right);
    update_down(cur, down);
    update_rot(cur, state, rotate);
    update_ard(cur, state, round);
}

// Get the shape of the piece according to the seed
char core::Piece::Shape(const uint8_t &seed) {
    const uint8_t idx = seed % kNStateInit;
    return kArrShapeInit.at(idx);
}

void core::Piece::Rotate() {
    // replace current with rotate
    cur = rotate;
    // update state
    state = next_state(state);

    // rotate go rotate
    update_rot(cur, state, rotate);

    // update left, right, down and around
    update_left(cur, left);
    update_right(cur, right);
    update_down(cur, down);
    update_ard(cur, state, round);
}

void core::Piece::Left() {
    // replace right with current
    right = cur;
    // replace current with left
    cur = left;
    // left move left
    update_left(cur, left);
    // update down, rotate and around
    update_down(cur, down);
    update_rot(cur, state, rotate);
    update_ard(cur, state, round);
}

void core::Piece::Right() {
    // replace left with current
    left = cur;
    // replace current with right
    cur = right;
    // right move right
    update_right(cur, right);
    // update down, rotate and around
    update_down(cur, down);
    update_rot(cur, state, rotate);
    update_ard(cur, state, round);
}

void core::Piece::Down() {
    // replace current with down
    cur = down;
    // down move down
    update_down(cur, down);
    // update left, right, rotate and around
    update_left(cur, left);
    update_right(cur, right);
    update_rot(cur, state, rotate);
    update_ard(cur, state, round);
}
