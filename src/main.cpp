#include "board.hpp"
#include "const.hpp"
#include "piece.hpp"
#include "term.hpp"

// collision / out of bound detection
// return false if it is a valid piece placement
static inline bool collide(const brick_t &indices, const field_t &mx) {
    // out of bound
    for (const auto &i : indices) {
        if (i == IDX_NA) {
            return true;
        }
    }
    // collision detection
    for (const auto &i : indices) {
        if (mx[i] != static_cast<uint8_t>(Board::Pixel::NUL)) {
            return true;
        }
    }
    return false;
}

// visualize the board matrix
static inline void to_str(const field_t &mx, Term::Screen &out) {
    for (uint8_t row = 0; row < HEIGHT; ++row) {
        std::string line = "";
        for (uint8_t col = 0; col < WIDTH; ++col) {
            uint8_t index = row * WIDTH + col;
            line += Board::px2str(mx[index]);
        }
        out[row] = line;
    }
}

// run one step of the game loop (piece falling)
void piece_step(Piece::Context &piece, Board::Context &board, bool &game_over) {
    if (duration_cast<sec>(steady_clock::now() - board.last_fall).count() >=
        1) {
        board.last_fall = steady_clock::now();
        // move down if possible
        // add to board otherwise
        if (!collide(piece.down, board.base)) {
            piece.Down();
        } else {
            // explode full rows
            board.RowsExplode();
            // initiate new piece
            piece.Spawn(board.Pop());
            // check if the game is over
            if (collide(piece.down, board.base)) {
                game_over = true;
            }
        }
    }
}

int main() {
    // Enable raw mode
    Term::Termios orig_termios;
    Term::enableRawMode(orig_termios);

    // Initialize the board
    bool game_over = false;
    Term::Screen screen;
    char ctrl;

    Board::Context board = Board::Context();
    Piece::Context piece;
    piece.Spawn(board.Pop());

    while (!game_over) {
        piece_step(piece, board, game_over);

        if (read(STDIN_FILENO, &ctrl, 1) > 0) {
            if (ctrl == 'h' && !collide(piece.left, board.base)) {
                piece.Left();
            } else if (ctrl == 'l' && !collide(piece.right, board.base)) {
                piece.Right();
            } else if (ctrl == 'k' && !collide(piece.rotate, board.base) &&
                       !collide(piece.round, board.base)) {
                piece.Rotate();
            } else if (ctrl == 'j' && !collide(piece.down, board.base)) {
                piece.Down();
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
        board.UpdateBoard(piece.cur);
        to_str(board.active, screen);

        // clear the screen
        Term::clearScreen();

        // print the board
        Term::printScreen(screen, board.lines, game_over);

        // Optional: Add a short delay for smoother animation
        usleep(50000); // 50 milliseconds
    }

    Term::disableRawMode(orig_termios);
    return 0;
}
