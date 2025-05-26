#pragma once

#include "config.h"
#include "const.hpp"
#include <fcntl.h>
#include <iomanip>
#include <iostream>
#include <termios.h>
#include <unistd.h>

namespace term {

    using Termios = struct termios;

    static inline void enableRawMode(Termios &orig_termios) {
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

        // Make stdin non-blocking
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }

    static inline void disableRawMode(const Termios &orig_termios) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    }

    // Clear the page and move cursor to home position
    static inline void clearScreen() { std::cout << "\033[2J\033[1;1H"; }

    static inline void
    printScreen(const std::array<std::string, HEIGHT> &screen, const char shape,
                const uint16_t &s, const bool &game_over) {
        // add top border
        std::cout << std::string(42, '#') << std::endl;
        for (uint8_t i = 0; i < screen.size(); ++i) {
            // add left separator
            std::cout << "##";
            // print line
            std::cout << screen[i];
            // add right separator
            std::cout << "##";
            if (i == 2) {
                std::cout << " VERSION: " << VERSION_MAJOR << "."
                          << VERSION_MINOR << "." << VERSION_PATCH << " "
                          << "##";
            } else if (i == 4) {
                std::cout << "  SCORE: " << std::setw(5) << s << "  "
                          << "##";
            } else if (i == 6) {
                std::cout << "   NEXT: " << std::setw(4) << shape << "   "
                          << "##";
            } else if (i == 10 && game_over) {
                std::cout << "    GAME OVER   "
                          << "##";
            } else {
                std::cout << std::string(16, ' ') << "##";
            }
            std::cout << std::endl;
        }
        // add bottom border
        std::cout << std::string(42, '#') << std::endl;
    }

    static inline void pause(char key) {
        while (true) {
            if (read(STDIN_FILENO, &key, 1) > 0 && key == 'p')
                break;
            usleep(1000000); // 1 second delay to avoid busy waiting
        }
    }

} // namespace term
