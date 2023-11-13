#include "vis.hpp"
#include "tetris.hpp"

void Vis::ToStr(const Pixels &src, const Dict &dict, str &out) {
    out = "";
    uint8_t old_row = 0;
    for (size_t i = 0; i < src.size(); ++i) {
        // add new line for each row
        uint8_t row = i / WIDTH;
        if (row > old_row) {
            out += "\n";
            old_row = row;
        }

        if (dict.find(src[i]) != dict.end()) {
            out += dict.at(src[i]);
        }
    }
}
