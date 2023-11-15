#include "move.hpp"
#include "term.hpp"
#include "tetris.hpp"
#include "vis.hpp"
#include <iostream>

// validate
bool Valid(const Indices &buffer, const Pixels &board) {
    // out of bound
    for (const auto &i : buffer) {
        if (i == MAX) {
            return false;
        }
    }
    // collision detection
    for (const auto &i : buffer) {
        if (board[i] != Canvas::Key::NUL) {
            return false;
        }
    }
    return true;
}

void autoDown(PieceContext &piece, GameContext &game, time_point &lastFall) {
    if (duration_cast<sec>(steady_clock::now() - lastFall).count() >= 1) {
        lastFall = steady_clock::now();
        // move down if possible
        // add to board otherwise
        if (Valid(piece.down, game.base)) {
            Move::Down(piece);
        } else {
            // add to board
            uint8_t idx = Vis::Reload(game);
            // check for full rows
            uint16_t rows = Vis::RowsExplode(game.active);
            // update to base board
            game.base = game.active;
            // update score
            game.lines += rows;
            // spawn new piece
            Move::Init(idx, piece);
        }
    }
}

int main() {
    GameContext game = Vis::InitGame();
    PieceContext piece;
    uint8_t idx = Vis::Reload(game);
    Move::Init(idx, piece);

    // Initialize the board
    std::string screen;

    Term::Termios orig_termios;
    Term::enableRawMode(orig_termios);

    time_point lastFall = steady_clock::now(); // Last time the piece moved down

    while (true) {
        // Game logic for moving down
        autoDown(piece, game, lastFall);

        char inputChar;
        if (read(STDIN_FILENO, &inputChar, 1) > 0) {
            if (inputChar == 'h' && Valid(piece.left, game.base)) {
                Move::Left(piece);
            }
            if (inputChar == 'l' && Valid(piece.right, game.base)) {
                Move::Right(piece);
            }
            if (inputChar == 'k' && Valid(piece.rotate, game.base) &&
                Valid(piece.round, game.base)) {
                Move::Rotate(piece);
            }
            if (inputChar == 'j' && Valid(piece.down, game.base)) {
                Move::Down(piece);
            }
        }

        // fill the board with blocks
        Vis::UpdateBoard(piece, game);

        // Clear the page and move cursor to home position
        std::cout << "\033[2J\033[1;1H";

        Vis::ToStr(game.active, Canvas::MAP, screen);
        std::cout << screen << std::endl;

        // Optional: Add a short delay for smoother animation
        usleep(10000); // 50 milliseconds
    }

    Term::disableRawMode(orig_termios);
    return 0;
}
