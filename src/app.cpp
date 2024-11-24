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

App::App(Mode mode, UI *ui, Engine *engine) : mode(mode), ui(ui), engine(engine) {}

void App::run() {
    if (mode == Mode::PLAY) {run_play();}
    else if (mode == Mode::BOT_VS_BOT) run_bot_vs_bot();
    else if (mode == Mode::BENCHMARK) run_benchmark();
}

void App::run_play() {
    Board last_board;
    Board current_board = Board::States::INITIAL;
    
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
            move = engine->search(current_board, at_turn);
            last_board = current_board;
            current_board.play_move(at_turn, move);
        }
        at_turn = !at_turn;
    }
    ui->display_message("\n" + std::to_string(white_score) + " " + std::to_string(black_score) + "\n");
}

void App::run_bot_vs_bot() {
    Board init_board = Board::States::INITIAL;
    uint64_t move = 0;

    bool color = false;
    while (true) {
        move = engine->search(init_board, color);

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
    uint64_t move = 0;
    move = engine->search(init_board, false);
    ui->display_board(init_board, move);
}
