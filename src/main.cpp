#include "core.hpp"
#include "term.hpp"

int main() {
    // Enable raw mode
    term::Termios orig_termios;
    term::enableRawMode(orig_termios);

    // Initialize the board
    bool game_over = false;
    std::array<std::string, HEIGHT> screen;
    char ctrl;

    core::Board b_ctx = core::Board();
    core::Piece p_ctx;
    p_ctx.Spawn(b_ctx.Pop());

    while (!game_over) {
        game_over = core::step(p_ctx, b_ctx);

        if (read(STDIN_FILENO, &ctrl, 1) > 0) {
            if (ctrl == 'h' && !core::collide(p_ctx.left, b_ctx.base)) {
                p_ctx.Left();
            } else if (ctrl == 'l' && !core::collide(p_ctx.right, b_ctx.base)) {
                p_ctx.Right();
            } else if (ctrl == 'k' &&
                       !core::collide(p_ctx.rotate, b_ctx.base) &&
                       !core::collide(p_ctx.round, b_ctx.base)) {
                p_ctx.Rotate();
            } else if (ctrl == 'j' && !core::collide(p_ctx.down, b_ctx.base)) {
                p_ctx.Down();
            } else if (ctrl == 'p') { // pause by pressing 'p'
                while (true) {
                    if (read(STDIN_FILENO, &ctrl, 1) > 0 && ctrl == 'p') {
                        break;
                    } else {
                        usleep(10000);
                    }
                }
            } else if (ctrl == 'q') { // quit by pressing 'q'
                game_over = true;
            } else { // otherwise beep
                std::cout << "\a";
            }
        }

        // update the active board
        b_ctx.UpdateActive(p_ctx.cur);
        core::ToString(b_ctx.active, screen);

        // clear the screen
        term::clearScreen();

        // print the board
        term::printScreen(screen, b_ctx.lines, game_over);

        // Optional: Add a short delay for smoother animation
        usleep(50000); // 50 milliseconds
    }

    term::disableRawMode(orig_termios);
    return 0;
}
