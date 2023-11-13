#pragma once

#include "tetris.hpp"
#include <iostream>
#include <map>

namespace Board {

    using str = std::string;
    using Dict = std::map<uint8_t, str>;
    using Indices = std::array<uint8_t, SIZE>;
    using Screen = std::array<str, HEIGHT>;

    namespace Ks {

        static constexpr uint8_t NUL = 0;
        static constexpr uint8_t BLK = 1;

    } // namespace Ks

    namespace Vs {

        static constexpr char NUL[] = "  ";
        static constexpr char BLK[] = "[]";

    } // namespace Vs

    static const Dict DICT = {
        {Board::Ks::NUL, Board::Vs::NUL},
        {Board::Ks::BLK, Board::Vs::BLK},
    };

    static constexpr Indices EMPTY_BOARD = {0};

    // visualize the board
    void visualize(const Indices &, const Dict &, str &);

} // namespace Board
