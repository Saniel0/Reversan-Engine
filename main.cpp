#include <iostream>
#include <random>
#include "reversi.h"

int main() {
    Board *init_board = new Board();
    Reversi *engine = new Reversi();

    /*init_board->load_benchmark_state();
    init_board->print_board();
    init_board->play_move(false, 1 << 5);
    init_board->print_moves(init_board->white_bitmap);
    init_board->print_moves(init_board->black_bitmap);
    init_board->print_board();
    return 0;*/

    init_board->load_benchmark_state();
    init_board->print_board();
    engine->find_best_move(init_board, false, 10);
    return 0;
    
    std::cout << "Setting up board.\n";
    init_board->load_start_state();
    init_board->print_board();

    int white_score = 0;
    int black_score = 0;

    bool at_turn = false;
    bool last_moved = true;
    while (true) {
        uint64_t possible_moves = init_board->find_moves(at_turn);
        if (possible_moves == 0) {
            if (!last_moved) {
                std::cout << "Game ended\n";
                int white = __builtin_popcountll(init_board->white_bitmap);
                int black = __builtin_popcountll(init_board->black_bitmap);
                if (white > black) {
                    white_score++;
                }
                else if (white < black) {
                    black_score++;
                }
                break;
            }
            last_moved = false;
            at_turn = !at_turn;
        }

        if (at_turn) {
            uint64_t move;
            move = engine->find_best_move(init_board, at_turn, 10);
            init_board->play_move(at_turn, move);
        }
        else {
            uint64_t move;
            move = engine->find_best_move(init_board, at_turn, 10);
            init_board->play_move(at_turn, move);
        }

        at_turn = !at_turn;
        init_board->print_board();
    }

    std::cout << '\n' << white_score << ' ' << black_score << '\n';
}
