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
#include <string>
#include <cstdlib>
#include "search.h"
#include "terminal.h"

#define MINIMAX 0
#define NEGASCOUT 1

void play_against_bot(int depth, int search);
void run_bot_vs_bot(int depth, int search);
void run_bench(int depth, int search);

int main(int argc, char *argv[]) {
    if (argc == 1) {
        std::cout << "No arguments provided. Use --help or -h for usage information." << '\n';
        return 1;
    }

    // launch options with default values
    int depth = 10;
    int search = NEGASCOUT;


    // Options parsing
    if (argc > 2) {
        for (int i = 2; i < argc; ++i) {
            std::string arg = argv[i];
            
            if (arg == "--depth" || arg == "-d") {
                if (i + 1 < argc) {
                    i++;
                    depth = std::atoi(argv[i]);
                    if (depth < 1 || depth > 50) {
                        std::cout << "Invalid depth. Use --help or -h for usage information." << '\n';
                        return 1;
                    }
                }
                else {
                    std::cout << "Flags --depth and -d requires an integer argument. Use --help or -h for usage information." << '\n';
                    return 1;
                }
            }

            else if (arg == "--search" || arg == "-s") {
                if (i + 1 < argc) {
                    i++;
                    arg = argv[i];
                    if (arg == "minimax") {
                        search = MINIMAX;
                    }
                    else if (arg == "negascout") {
                        search = NEGASCOUT;
                    }
                    else {
                        std::cout << "Invalid search algorithm. Use --help or -h for usage information." << '\n';
                        return 1;
                    }
                }
                else {
                    std::cout << "Flags --search and -s requires an integer argument. Use --help or -h for usage information." << '\n';
                    return 1;
                }
            }

            else {
                std::cout << "Invalid option. Use --help or -h for usage information." << '\n';
                return 1;
            }
        }
    }

    // Mode parsing
    std::string mode = argv[1];
    if (mode == "--play") {
        play_against_bot(depth, search);
    }
    else if (mode == "--bot-vs-bot") {
        run_bot_vs_bot(depth, search);
    }
    else if (mode == "--benchmark") {
        run_bench(depth, search);
    }
    else if (mode == "--help" || mode == "-h") {
        print_help();
    }
    else {
        std::cout << "Invalid mode. Use --help or -h for usage information." << '\n';
        return 1;
    }
    
    return 0;
}

void run_bench(int depth, int search) {
    Board init_board;
    Search engine;
    uint64_t move = 0;
    
    init_board.load_benchmark_state();
    if (search == MINIMAX) {
        move = engine.start_minimax(init_board, false, depth);
    }
    else if (search == NEGASCOUT) {
        move = engine.start_negascout(init_board, false, depth);
    }
    print_board_moves(&init_board, move);
}

void run_bot_vs_bot(int depth, int search) {
    Board init_board;
    Search engine;
    uint64_t move = 0;

    init_board.load_start_state();
    bool color = false;
    while (true) {
        if (search == MINIMAX) {
            move = engine.start_minimax(init_board, color, depth);
        }
        else if (search == NEGASCOUT) {
            move = engine.start_negascout(init_board, color, depth);
        }

        if (move == 0) {
            break;
        }

        init_board.play_move(color, move);
        color = !color;
    }
    print_board(&init_board);
}

void play_against_bot(int depth, int search) {
    print_title_blur();
    Board last_board;
    Board current_board;
    Search engine;
    
    current_board.load_start_state();
    int white_score = 0;
    int black_score = 0;
    bool at_turn = false;
    bool last_moved = true;

    print_boards(&last_board, &current_board, at_turn);
    std::cout << "\n";
    while (true) {
        uint64_t possible_moves = current_board.find_moves(at_turn);
        if (possible_moves == 0) {
            if (!last_moved) {
                std::cout << "Game ended\n";
                white_score = __builtin_popcountll(current_board.white_bitmap);
                black_score = __builtin_popcountll(current_board.black_bitmap);
                break;
            }
            last_moved = false;
            at_turn = !at_turn;
            continue;
        }
        last_moved = true;
        if (at_turn) {
            delete_lines(13);
            print_boards(&last_board, &current_board, at_turn);
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
            last_board = current_board;
            current_board.play_move(at_turn, move);
        }
        else {
            uint64_t move = 0;
            if (search == MINIMAX) {
                move = engine.start_minimax(current_board, at_turn, depth);
            }
            else if (search == NEGASCOUT) {
                move = engine.start_negascout(current_board, at_turn, depth);
            }
            last_board = current_board;
            current_board.play_move(at_turn, move);
        }
        at_turn = !at_turn;
    }
    std::cout << '\n' << white_score << ' ' << black_score << '\n';
}
