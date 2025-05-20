#include "board.hpp"
#include "const.hpp"
#include "piece.hpp"
#include "term.hpp"

// visualize the board matrix
static inline void to_str(const field_t &mx, term::Screen &out) {
    for (uint8_t row = 0; row < HEIGHT; ++row) {
        std::string line = "";
        for (uint8_t col = 0; col < WIDTH; ++col) {
            uint8_t index = row * WIDTH + col;
            line += board::px2str(mx[index]);
        }
        out[row] = line;
    }
}

// run one step of the game loop (piece falling)
void piece_step(piece::Context &p_ctx, board::Context &b_ctx, bool &game_over) {
    if (duration_cast<sec>(steady_clock::now() - b_ctx.last_fall).count() >=
        1) {
        b_ctx.last_fall = steady_clock::now();
        // move down if possible
        // add to board otherwise
        if (!board::collide(p_ctx.down, b_ctx.base)) {
            p_ctx.Down();
        } else {
            // explode full rows
            b_ctx.RowsExplode();
            // initiate new piece
            p_ctx.Spawn(b_ctx.Pop());
            // check if the game is over
            if (board::collide(p_ctx.down, b_ctx.base)) {
                game_over = true;
            }
        }
    }
}

int main() {
    // Enable raw mode
    term::Termios orig_termios;
    term::enableRawMode(orig_termios);

    // Initialize the board
    bool game_over = false;
    term::Screen screen;
    char ctrl;

    board::Context b_ctx = board::Context();
    piece::Context p_ctx;
    p_ctx.Spawn(b_ctx.Pop());

    while (!game_over) {
        piece_step(p_ctx, b_ctx, game_over);

        if (read(STDIN_FILENO, &ctrl, 1) > 0) {
            if (ctrl == 'h' && !board::collide(p_ctx.left, b_ctx.base)) {
                p_ctx.Left();
            } else if (ctrl == 'l' &&
                       !board::collide(p_ctx.right, b_ctx.base)) {
                p_ctx.Right();
            } else if (ctrl == 'k' &&
                       !board::collide(p_ctx.rotate, b_ctx.base) &&
                       !board::collide(p_ctx.round, b_ctx.base)) {
                p_ctx.Rotate();
            } else if (ctrl == 'j' && !board::collide(p_ctx.down, b_ctx.base)) {
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

        // fill the board with blocks
        b_ctx.UpdateBoard(p_ctx.cur);
        to_str(b_ctx.active, screen);

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
