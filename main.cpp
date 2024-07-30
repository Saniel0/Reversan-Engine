#include <iostream>
#include <random>
#include "reversi.h"

void print_title() {
    std::cout << "          _____                    _____                    _____                    _____                    _____                    _____                    _____          \n";
    std::cout << "         /\\    \\                  /\\    \\                  /\\    \\                  /\\    \\                  /\\    \\                  /\\    \\                  /\\    \\         \n";
    std::cout << "        /::\\    \\                /::\\    \\                /::\\____\\                /::\\    \\                /::\\    \\                /::\\    \\                /::\\    \\        \n";
    std::cout << "       /::::\\    \\              /::::\\    \\              /:::/    /               /::::\\    \\              /::::\\    \\              /::::\\    \\               \\:::\\    \\       \n";
    std::cout << "      /::::::\\    \\            /::::::\\    \\            /:::/    /               /::::::\\    \\            /::::::\\    \\            /::::::\\    \\               \\:::\\    \\      \n";
    std::cout << "     /:::/\\:::\\    \\          /:::/\\:::\\    \\          /:::/    /               /:::/\\:::\\    \\          /:::/\\:::\\    \\          /:::/\\:::\\    \\               \\:::\\    \\     \n";
    std::cout << "    /:::/__\\:::\\    \\        /:::/__\\:::\\    \\        /:::/____/               /:::/__\\:::\\    \\        /:::/__\\:::\\    \\        /:::/__\\:::\\    \\               \\:::\\    \\    \n";
    std::cout << "   /::::\\   \\:::\\    \\      /::::\\   \\:::\\    \\       |::|    |               /::::\\   \\:::\\    \\      /::::\\   \\:::\\    \\       \\:::\\   \\:::\\    \\              /::::\\    \\   \n";
    std::cout << "  /::::::\\   \\:::\\    \\    /::::::\\   \\:::\\    \\      |::|    |     _____    /::::::\\   \\:::\\    \\    /::::::\\   \\:::\\    \\    ___\\:::\\   \\:::\\    \\    ____    /::::::\\    \\  \n";
    std::cout << " /:::/\\:::\\   \\:::\\____\\  /:::/\\:::\\   \\:::\\    \\     |::|    |    /\\    \\  /:::/\\:::\\   \\:::\\    \\  /:::/\\:::\\   \\:::\\____\\  /\\   \\:::\\   \\:::\\    \\  /\\   \\  /:::/\\:::\\    \\ \n";
    std::cout << "/:::/  \\:::\\   \\:::|    |/:::/__\\:::\\   \\:::\\____\\    |::|    |   /::\\____\\/:::/__\\:::\\   \\:::\\____\\/:::/  \\:::\\   \\:::|    |/::\\   \\:::\\   \\:::\\____\\/::\\   \\/:::/  \\:::\\____\\\n";
    std::cout << "\\::/   |::::\\  /:::|____|\\:::\\   \\:::\\   \\::/    /    |::|    |  /:::/    /\\:::\\   \\:::\\   \\::/    /\\::/   |::::\\  /:::|____|\\:::\\   \\:::\\   \\::/    /\\:::\\  /:::/    \\::/    /\n";
    std::cout << " \\/____|:::::\\/:::/    /  \\:::\\   \\:::\\   \\/____/     |::|    | /:::/    /  \\:::\\   \\:::\\   \\/____/  \\/____|:::::\\/:::/    /  \\:::\\   \\:::\\   \\/____/  \\:::\\/:::/    / \\/____/ \n";
    std::cout << "       |:::::::::/    /    \\:::\\   \\:::\\    \\         |::|____|/:::/    /    \\:::\\   \\:::\\    \\            |:::::::::/    /    \\:::\\   \\:::\\    \\       \\::::::/    /          \n";
    std::cout << "       |::|\\::::/    /      \\:::\\   \\:::\\____\\        |:::::::::::/    /      \\:::\\   \\:::\\____\\           |::|\\::::/    /      \\:::\\   \\:::\\____\\       \\::::/____/           \n";
    std::cout << "       |::| \\::/____/        \\:::\\   \\::/    /        \\::::::::::/____/        \\:::\\   \\::/    /           |::| \\::/____/        \\:::\\  /:::/    /        \\:::\\    \\           \n";
    std::cout << "       |::|  ~|               \\:::\\   \\/____/          ~~~~~~~~~~               \\:::\\   \\/____/            |::|  ~|               \\:::\\/:::/    /          \\:::\\    \\          \n";
    std::cout << "       |::|   |                \\:::\\    \\                                        \\:::\\    \\                |::|   |                \\::::::/    /            \\:::\\    \\         \n";
    std::cout << "       \\::|   |                 \\:::\\____\\                                        \\:::\\____\\               \\::|   |                 \\::::/    /              \\:::\\____\\        \n";
    std::cout << "        \\:|   |                  \\::/    /                                         \\::/    /                \\:|   |                  \\::/    /                \\::/    /        \n";
    std::cout << "         \\|___|                   \\/____/                                           \\/____/                  \\|___|                   \\/____/                  \\/____/         \n";
}

void print_title_alt() {
    std::cout << "░▒▓███████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓███████▓▒░ ░▒▓███████▓▒░▒▓█▓▒░\n";
    std::cout << "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░\n";
    std::cout << "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░       ░▒▓█▓▒▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░\n";
    std::cout << "░▒▓███████▓▒░░▒▓██████▓▒░  ░▒▓█▓▒▒▓█▓▒░░▒▓██████▓▒░ ░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░\n";
    std::cout << "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░\n";
    std::cout << "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░\n";
    std::cout << "░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░  ░▒▓██▓▒░  ░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░░▒▓█▓▒░\n";
}

void play_game() {
    
}

int main() {
    print_title_alt();
    Board *init_board = new Board();
    Reversi *engine = new Reversi();

    /*init_board->load_start_state();
    uint64_t next_move;
    bool colour = false;
    while ((next_move = engine->find_best_move(init_board, colour, 10)) != 0) {
        init_board->play_move(colour, next_move);
        //init_board->print_board();
        if (colour) {
            colour = false;
        }
        else {
            colour = true;
        }
    }
    init_board->print_board();
    return 0;*/
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
            std::cout << !at_turn << " cannot move\n";
        }

        if (at_turn) {
            init_board->print_board_moves(possible_moves);
            uint64_t move;
            while (true) {
                std::cout << "Select move > ";
                int x, y;
                std::cin >> x >> y;
                move = 1L << (63 - (y*8 + x));
                if (move & possible_moves) {
                    break;
                }
                std::cout << "Invalid move!" << '\n';
            }
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
