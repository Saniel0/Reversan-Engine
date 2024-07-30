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

void run_bench() {
    Board *init_board = new Board();
    Reversi *engine = new Reversi();
    init_board->load_benchmark_state();
    init_board->print_board();
    engine->find_best_move(init_board, false, 10);
}

void run_bot_vs_bot_bench() {
    Board *init_board = new Board();
    Reversi *engine = new Reversi();
    init_board->load_start_state();
    uint64_t next_move;
    bool colour = false;
    while ((next_move = engine->find_best_move(init_board, colour, 10)) != 0) {
        init_board->play_move(colour, next_move);
        if (colour) {
            colour = false;
        }
        else {
            colour = true;
        }
    }
    init_board->print_board();
}

void play_against_bot() {
    Board *init_board = new Board();
    Reversi *engine = new Reversi();
    
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
                white_score = __builtin_popcountll(init_board->white_bitmap);
                black_score = __builtin_popcountll(init_board->black_bitmap);
                break;
            }
            last_moved = false;
            at_turn = !at_turn;
            std::cout << !at_turn << " cannot move\n";
            continue;
        }
        last_moved = true;
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

int main() {
    print_title_alt();
    run_bot_vs_bot_bench();
}
