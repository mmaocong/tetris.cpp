#include "vis.hpp"

void Vis::ToStr(const Pixels &src, const Dict &dict, Screen &out) {
    for (uint8_t row = 0; row < HEIGHT; ++row) {
        std::string line = "";
        for (uint8_t col = 0; col < WIDTH; ++col) {
            uint8_t index = row * WIDTH + col;
            if (dict.find(src[index]) != dict.end()) {
                line += dict.at(src[index]);
            }
        }
        out[row] = line;
    }
}
