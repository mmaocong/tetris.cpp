#include "board.hpp"
#include "pieces.hpp"
#include "term.hpp"

void autoDown(Pieces::Context &ctx, time_point &lastFall) {
    if (duration_cast<sec>(steady_clock::now() - lastFall).count() >= 1) {
        lastFall = steady_clock::now();
        // increase the y position if not at the bottom
        Pieces::GoDown(ctx);
    }
}

void addPiece(const Pieces::Context &ctx, Board::Indices &dst) {
    // Place Tetromino on output board
    // for (const auto &idx : ctx.coord) {
    //     dst[idx] = Board::Ks::BLK;
    // }
    for (uint8_t i = 0; i < PIECE_SIZE; i++) {
        dst[ctx.coord[i]] = Board::Ks::BLK;
    }
}

int main() {
    Pieces::Context piece = {Pieces::InitCoords::Z};
    Pieces::InitContext(piece);

    // Initialize the board
    Board::Indices board = Board::EMPTY_BOARD;
    Board::Indices tmpBoard;
    std::string screen;

    Term::Termios orig_termios;
    Term::enableRawMode(orig_termios);

    time_point lastFall = steady_clock::now(); // Last time the piece moved down

    while (true) {
        // Game logic for moving down
        autoDown(piece, lastFall);

        char inputChar;
        if (read(STDIN_FILENO, &inputChar, 1) > 0) {
            if (inputChar == 'h' && true) {
                Pieces::GoLeft(piece);
            }
            if (inputChar == 'l' && true) {
                Pieces::GoRight(piece);
            }
            if (inputChar == 'k') {
                Pieces::GoRotate(piece);
            }
        }

        // fill the board with blocks
        tmpBoard = board;
        addPiece(piece, tmpBoard);

        // Clear the page and move cursor to home position
        std::cout << "\033[2J\033[1;1H";

        Board::visualize(tmpBoard, Board::DICT, screen);
        std::cout << screen << std::endl;

        // Optional: Add a short delay for smoother animation
        usleep(10000); // 50 milliseconds
    }

    Term::disableRawMode(orig_termios);
    return 0;
}
