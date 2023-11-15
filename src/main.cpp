#include "consts.hpp"
#include "move.hpp"
#include "term.hpp"
#include "types.hpp"
#include "vis.hpp"
#include <iostream>

// game logic of auto falling
void Fall(PieceContext &piece, GameContext &game, bool &game_over) {
    if (duration_cast<sec>(steady_clock::now() - game.last_fall).count() >= 1) {
        game.last_fall = steady_clock::now();
        // move down if possible
        // add to board otherwise
        if (Move::Valid(piece.down, game.base)) {
            Move::Down(piece);
        } else {
            // explode full rows
            Vis::RowsExplode(game);
            // initiate new piece
            Move::Spawn(Vis::Pop(game), piece);
            // check if the game is over
            if (!Move::Valid(piece.down, game.base)) {
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
    Screen board;
    char ctrl;

    GameContext game = Vis::InitGame();
    PieceContext piece;
    Move::Spawn(Vis::Pop(game), piece);

    while (!game_over) {
        Fall(piece, game, game_over);

        if (read(STDIN_FILENO, &ctrl, 1) > 0) {
            if (ctrl == 'h' && Move::Valid(piece.left, game.base)) {
                Move::Left(piece);
            } else if (ctrl == 'l' && Move::Valid(piece.right, game.base)) {
                Move::Right(piece);
            } else if (ctrl == 'k' && Move::Valid(piece.rotate, game.base) &&
                       Move::Valid(piece.round, game.base)) {
                Move::Rotate(piece);
            } else if (ctrl == 'j' && Move::Valid(piece.down, game.base)) {
                Move::Down(piece);
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
        Vis::UpdateBoard(piece, game);
        Vis::ToStr(game.active, Canvas::MAP, board);

        // clear the screen
        Term::clearScreen();

        // print the board
        Term::printScreen(board, game.lines, game_over);

        // Optional: Add a short delay for smoother animation
        usleep(50000); // 50 milliseconds
    }

    Term::disableRawMode(orig_termios);
    return 0;
}
