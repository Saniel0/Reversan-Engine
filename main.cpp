#include <iostream>
#include "reversi.h"

int main() {
    Board *init_board = new Board();
    Reversi *engine = new Reversi();
    
    /*int moves_count = 0;
    init_board->load_start_state();
    uint64_t valid_moves = init_board->find_moves(false);
    init_board->print_board();
    //init_board->print_board_moves(valid_moves);
    init_board->print_moves(valid_moves);
    return 0;*/

    /*init_board->load_benchmark_state();
    init_board->print_board();
    int result = engine->heuristics(init_board, 0, false);
    std::cout << result << '\n';
    return 0;*/

    init_board->load_benchmark_state();
    //init_board->load_start_state();
    init_board->print_board();
    engine->find_best_move(init_board, false);
    return 0;
    
    std::cout << "Setting up board.\n";
    init_board->load_start_state();
    init_board->print_board();

    /*int x, y;
    int moves_count = 0;
    uint64_t move, valid_moves;
    bool color = true;
    while (true) {
        // human at turn
        color = !color;
        valid_moves = init_board->find_moves(color, moves_count);
        init_board->print_board_moves(valid_moves);
        std::cout << "Your turn! Select move > ";
        std::cin >> x;
        std::cin >> y;
        move = static_cast<uint64_t>(1) << (63 - (x * 8 + y));
        if (valid_moves & move) {
            std::cout << "You played move " << x << ' ' << y << '\n';
            init_board->play_move(color, move);
            std::cout << "-------------------------------------------\n";
        }
        else {
            std::cout << "Invalid move! Choose valid move.\n";
            continue;
        }
    }*/

    /*int x, y;
    int moves_count = 0;
    uint64_t move, valid_moves;
    bool color = true;
    while (true) {
         // AI at turn
        std::cout << "AI at turn!\n";
        valid_moves = init_board->find_moves(false, moves_count);
        init_board->print_board_moves(valid_moves);
        move = engine->find_best_move(init_board, false);
        if (valid_moves & move) {
            init_board->play_move(false, move);
            valid_moves = init_board->find_moves(true, moves_count);
            init_board->print_board_moves(valid_moves);
            std::cout << "-------------------------------------------\n";
        }
        else {
            std::cout << "ERROR - AI CHOSE INVALID MOVE.\n";
            return 0;
        }

        // human at turn
        //color = !color;
        valid_moves = init_board->find_moves(true, moves_count);
        std::cout << "Your turn! Select move > ";
        std::cin >> x;
        std::cin >> y;
        move = static_cast<uint64_t>(1) << (63 - (x * 8 + y));
        if (valid_moves & move) {
            std::cout << "You played move " << x << ' ' << y << '\n';
            init_board->play_move(true, move);
            std::cout << "-------------------------------------------\n";
        }
        else {
            std::cout << "Invalid move! Choose valid move.\n";
            continue;
        }
    }*/
}
