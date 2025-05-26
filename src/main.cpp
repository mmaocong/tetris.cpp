#include "core.hpp"
#include "term.hpp"

void handle_key(char key, core::Piece &p, const core::Board &b, bool &over) {
    if (key == 'h' && !core::collide(p.left, b.base)) {
        p.Left();
    } else if (key == 'l' && !core::collide(p.right, b.base)) {
        p.Right();
    } else if (key == 'k' && !core::collide(p.rotate, b.base) &&
               !core::collide(p.round, b.base)) {
        p.Rotate();
    } else if (key == 'j' && !core::collide(p.down, b.base)) {
        p.Down();
    } else if (key == 'p') { // pause by pressing 'p'
        term::pause(key);
    } else if (key == 'q') { // quit by pressing 'q'
        over = true;
    }
    // // avoid otherwise beep
    // else {
    //     std::cout << "\a";
    // }
}

int main() {
    // Enable raw mode
    term::Termios orig_termios;
    term::enableRawMode(orig_termios);

    // Initialize the board
    bool game_over = false;
    std::array<std::string, HEIGHT> screen;
    char key;

    core::Board b = core::Board();
    core::Piece p;
    p.Spawn(b.Pop());

    while (!game_over) {
        game_over = core::step(p, b);

        if (read(STDIN_FILENO, &key, 1) > 0) {
            handle_key(key, p, b, game_over);
        }

        // update the active board
        b.UpdateActive(p.cur);
        core::ToString(b.active, screen);

        // clear the screen and print the current state
        term::clearScreen();
        term::printScreen(screen, p.Shape(b.next), b.lines, game_over);

        usleep(10000); // ~100 FPS
    }

    term::disableRawMode(orig_termios);
    return 0;
}
