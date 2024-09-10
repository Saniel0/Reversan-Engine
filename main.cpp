/*
    This file is part of Reversan Engine.

    Reversan Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Reversan Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Reversan Engine. If not, see <https://www.gnu.org/licenses/>. 
*/

#include <iostream>
#include <random>
#include "reversi.h"
#include "terminal.h"

void run_bench() {
    Board *init_board = new Board();
    Reversi *engine = new Reversi();
    init_board->load_benchmark_state();
    init_board->print_board();
    engine->start_negascout(init_board, false, 10);
}

void run_bot_vs_bot_bench() {
    Board *init_board = new Board();
    Reversi *engine = new Reversi();
    init_board->load_start_state();
    uint64_t next_move;
    bool colour = false;
    while ((next_move = engine->start_negascout(init_board, colour, 10)) != 0) {
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
    Board *last_board = new Board();
    Board *current_board = new Board();
    Reversi *engine = new Reversi();
    
    current_board->load_start_state();
    int white_score = 0;
    int black_score = 0;
    bool at_turn = false;
    bool last_moved = true;

    print_boards(last_board, current_board, at_turn);
    std::cout << "\n";
    while (true) {
        uint64_t possible_moves = current_board->find_moves(at_turn);
        if (possible_moves == 0) {
            if (!last_moved) {
                std::cout << "Game ended\n";
                white_score = __builtin_popcountll(current_board->white_bitmap);
                black_score = __builtin_popcountll(current_board->black_bitmap);
                break;
            }
            last_moved = false;
            at_turn = !at_turn;
            continue;
        }
        last_moved = true;
        if (at_turn) {
            delete_lines(13);
            print_boards(last_board, current_board, at_turn);
            uint64_t move;
            while (true) {
                std::cout << "Select move > ";
                int x, y;
                std::cin >> x >> y;
                move = 1L << (63 - (y*8 + x));
                if (move & possible_moves) {
                    break;
                }
                delete_lines(1);
                std::cout << "INVALID - ";
            }
            last_board->copy_state(current_board);
            current_board->play_move(at_turn, move);
        }
        else {
            uint64_t move;
            move = engine->start_negascout(current_board, at_turn, 10);
            last_board->copy_state(current_board);
            current_board->play_move(at_turn, move);
        }
        at_turn = !at_turn;
    }
    std::cout << '\n' << white_score << ' ' << black_score << '\n';
}

int main() {
    print_title_blur();
    play_against_bot();
    //run_bench();
    //run_bot_vs_bot_bench();
    return 0;
}
