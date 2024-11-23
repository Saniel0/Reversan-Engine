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

#include "app.h"
#include "search.h"

App::App(UI *ui) : settings{Modes::PLAY, 10, Engine::NEGASCOUT}, ui(ui) {}

App::App(UI *ui, int mode, int search_depth, int search_method) : settings{mode, search_depth, search_method}, ui(ui) {}

void App::run() {
    if (settings.mode == Modes::PLAY) {run_play();}
    else if (settings.mode == Modes::BOT_VS_BOT) run_bot_vs_bot();
    else if (settings.mode == Modes::BENCHMARK) run_benchmark();
}

void App::run_play() {
    Board last_board;
    Board current_board = Board::States::INITIAL;
    Search engine;
    
    int white_score = 0;
    int black_score = 0;
    bool at_turn = false;
    bool last_moved = true;

    ui->display_game(last_board, current_board, at_turn);
    while (true) {
        uint64_t possible_moves = current_board.find_moves(at_turn);
        if (possible_moves == 0) {
            if (!last_moved) {
                ui->display_message("Game ended");
                white_score = current_board.count_white();
                black_score = current_board.count_black();
                break;
            }
            last_moved = false;
            at_turn = !at_turn;
            continue;
        }
        last_moved = true;
        if (at_turn) {
            ui->display_game(last_board, current_board, at_turn);
            uint64_t move;
            while (true) {
                ui->display_message("Select move>");
                UI::UserInput input = ui->get_input();
                move = 1L << (63 - (input.y*8 + input.x));
                if (move & possible_moves) {
                    break;
                }
                ui->display_game(last_board, current_board, at_turn);
                ui->display_message("INVALID");
            }
            last_board = current_board;
            current_board.play_move(at_turn, move);
        }
        else {
            uint64_t move = 0;
            if (settings.search_method == Engine::MINIMAX) {
                move = engine.start_minimax(current_board, at_turn, settings.search_depth);
            }
            else if (settings.search_method == Engine::NEGASCOUT) {
                move = engine.start_negascout(current_board, at_turn, settings.search_depth);
            }
            last_board = current_board;
            current_board.play_move(at_turn, move);
        }
        at_turn = !at_turn;
    }
    ui->display_message("\n" + std::to_string(white_score) + " " + std::to_string(black_score) + "\n");
}

void App::run_bot_vs_bot() {
    Board init_board = Board::States::INITIAL;
    Search engine;
    uint64_t move = 0;

    bool color = false;
    while (true) {
        if (settings.search_method == Engine::MINIMAX) {
            move = engine.start_minimax(init_board, color, settings.search_depth);
        }
        else if (settings.search_method == Engine::NEGASCOUT) {
            move = engine.start_negascout(init_board, color, settings.search_depth);
        }

        if (move == 0) {
            break;
        }

        init_board.play_move(color, move);
        color = !color;
    }
    ui->display_board(init_board, 0);
}

void App::run_benchmark() {
    Board init_board = Board::States::BENCHMARK;
    Search engine;
    uint64_t move = 0;
    
    if (settings.search_method == Engine::MINIMAX) {
        move = engine.start_minimax(init_board, false, settings.search_depth);
    }
    else if (settings.search_method == Engine::NEGASCOUT) {
        move = engine.start_negascout(init_board, false, settings.search_depth);
    }
    ui->display_board(init_board, move);
}

bool App::parse_settings(int argc, char **argv) {
    // check if any arguments were provided (first is just executable name)
    if (argc == 1) {
        ui->display_error_message("No arguments provided. Use --help or -h for usage information.");
        return false;
    }

    // parse app mode
    int mode;
    std::string mode_str = argv[1];
    if (mode_str == "--play") mode = Modes::PLAY;
    else if (mode_str == "--bot-vs-bot") mode = Modes::BOT_VS_BOT;
    else if (mode_str == "--benchmark") mode = Modes::BENCHMARK;
    else if (mode_str == "--help" || mode_str == "-h") {
        ui->display_help();
        return false;
    }
    else {
        ui->display_error_message("Invalid mode. Use --help or -h for usage information.");
        return false;
    }

    // set default options
    int depth = 10;
    int search = Engine::NEGASCOUT;

    // parse options
    if (argc > 2) {
        for (int i = 2; i < argc; ++i) {
            std::string arg = argv[i];
            // parse depth flag
            if (arg == "--depth" || arg == "-d") {
                if (i + 1 < argc) {
                    i++;
                    depth = std::atoi(argv[i]);
                    if (depth < 1 || depth > 50) {
                        ui->display_error_message("Invalid depth. Use --help or -h for usage information.");
                        return false;
                    }
                }
                else {
                    ui->display_error_message("Flags --depth and -d requires an integer argument. Use --help or -h for usage information.");
                    return false;
                }
            }
            // parse search flag
            else if (arg == "--search" || arg == "-s") {
                if (i + 1 < argc) {
                    i++;
                    arg = argv[i];
                    if (arg == "minimax") {
                        search = Engine::MINIMAX;
                    }
                    else if (arg == "negascout") {
                        search = Engine::NEGASCOUT;
                    }
                    else {
                        ui->display_error_message("Invalid search algorithm. Use --help or -h for usage information.");
                        return false;
                    }
                }
                else {
                    ui->display_error_message("Flags --search and -s requires an integer argument. Use --help or -h for usage information.");
                    return false;
                }
            }
            // invalid option
            else {
                ui->display_error_message("Invalid option. Use --help or -h for usage information.");
                return false;
            }
        }
    }

    // save the parsed settings
    settings = {mode, depth, search};
    return true;
}
