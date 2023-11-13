#pragma once

#include <fcntl.h>
#include <map>
#include <termios.h>
#include <unistd.h>

typedef std::chrono::seconds sec;
typedef std::chrono::steady_clock steady_clock;
typedef std::chrono::time_point<steady_clock> time_point;

namespace Term {

    using Termios = struct termios;

    inline void enableRawMode(Termios &orig_termios) {
        tcgetattr(STDIN_FILENO, &orig_termios);
        struct termios raw = orig_termios;
        raw.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

        // Make stdin non-blocking
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }

    inline void disableRawMode(const Termios &orig_termios) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    }

} // namespace Term
