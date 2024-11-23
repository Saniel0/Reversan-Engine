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

#include "terminal.h"
#include <iostream>

Terminal::Terminal() {
    std::cout << Escapes::SWITCH_BUFFER;
}

Terminal::~Terminal() {
    std::cout << Escapes::RESTORE_BUFFER;
}

void Terminal::clear_terminal() const {
    std::cout << Escapes::CLEAR_BUFFER;
}

UI::UserInput Terminal::get_input() {
    UserInput input;
    std::cin >> input.x >> input.y;
    return input;
}

void Terminal::print_title() const {
    std::cout
        << "░▒▓███████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓███████▓▒░ ░▒▓███████▓▒░░▒▓██████▓▒░░▒▓███████▓▒░\n"
        << "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░\n"
        << "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░       ░▒▓█▓▒▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░\n"
        << "░▒▓███████▓▒░░▒▓██████▓▒░  ░▒▓█▓▒▒▓█▓▒░░▒▓██████▓▒░ ░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░\n"
        << "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░\n"
        << "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░\n"
        << "░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░  ░▒▓██▓▒░  ░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░\n";
}

void Terminal::display_help() {
    std::cout 
        << Escapes::RESTORE_BUFFER // print to original buffer
        << "Usage: reversan [MODE] [OPTIONS]\n"
        << "\n"
        << "Modes:\n"
        << "--help, -h                           Display this help message.\n"
        <<"--play                               Play against the engine in terminal interface.\n"
        << "--bot-vs-bot                         Start game where the engine plays against itself.\n"
        << "--benchmark                          Run search on pre-defined state.\n"
        << "\n"
        << "Additional Options:\n"
        << "--depth, -d <1 - 49>                 Set the engine's search depth (default: 10).\n"
        << "--search, -s <negascout | minimax>   Choose the tree search algorithm (default: negascout).\n"
        << Escapes::SWITCH_BUFFER; // switch back to temporary buffer
}

void Terminal::display_message(std::string str) {
    std::cout << str << '\n';
}

void Terminal::display_error_message(std::string str) {
    std::cout << Escapes::RESTORE_BUFFER << str << '\n' << Escapes::SWITCH_BUFFER;
}

void Terminal::display_board(Board &board, uint64_t moves) {
    uint64_t bit_mask = static_cast<uint64_t>(1) << 63;
    std::cout << Escapes::BOLD << "  0 1 2 3 4 5 6 7\n" << Escapes::COLOR_RESET;
    for (int i = 0; i < 8; ++i) {
        std::cout << Escapes::BOLD << i << ' ' << Escapes::COLOR_RESET;
        for (int j = 0; j < 8; ++j) {
            if (board.white() & bit_mask) {
                std::cout << Escapes::YELLOW << "X " << Escapes::COLOR_RESET;
            }
            else if (board.black() & bit_mask) {
                std::cout << Escapes::GREEN << "O " << Escapes::COLOR_RESET;
            }
            else if (moves & bit_mask) {
                std::cout << Escapes::RED << "+ " << Escapes::COLOR_RESET;
            }
            else {
                std::cout << "- ";
            }
            bit_mask >>= 1;
        }
        std::cout << '\n';
    }
}

void Terminal::display_game(Board &last_state, Board &current_state, bool current_color) {
    clear_terminal();
    print_title();
    uint64_t bit_mask1 = static_cast<uint64_t>(1) << 63;
    uint64_t bit_mask2 = static_cast<uint64_t>(1) << 63;
    uint64_t bit_mask3 = static_cast<uint64_t>(1) << 63;
    int white_score = current_state.count_white();
    int black_score = current_state.count_black();
    std::cout << Escapes::BOLD << "  0 1 2 3 4 5 6 7        0 1 2 3 4 5 6 7     |       0 1 2 3 4 5 6 7\n" << Escapes::COLOR_RESET;
    for (int i = 0; i < 8; ++i) {
        std::cout << Escapes::BOLD << i << ' ' << Escapes::COLOR_RESET;
        for (int j = 0; j < 8; ++j) {
            if (last_state.white() & bit_mask1) {
                std::cout << Escapes::YELLOW << "X " << Escapes::COLOR_RESET;
            }
            else if (last_state.black() & bit_mask1) {
                std::cout << Escapes::GREEN << "O " << Escapes::COLOR_RESET;
            }
            else if (last_state.find_moves(!current_color) & bit_mask1) {
                std::cout << Escapes::RED << "+ " << Escapes::COLOR_RESET;
            }
            else {
                std::cout << "- ";
            }
            bit_mask1 >>= 1;
        }
        std::cout << Escapes::BOLD << "     " << i << ' ' << Escapes::COLOR_RESET;
        for (int j = 0; j < 8; ++j) {
            if (current_state.white() & bit_mask2) {
                std::cout << Escapes::YELLOW << "X " << Escapes::COLOR_RESET;
            }
            else if (current_state.black() & bit_mask2) {
                std::cout << Escapes::GREEN << "O " << Escapes::COLOR_RESET;
            }
            else {
                std::cout << "- ";
            }
            bit_mask2 >>= 1;
        }
        std::cout << Escapes::BOLD << "    |     " << i << ' ' << Escapes::COLOR_RESET;
        for (int j = 0; j < 8; ++j) {
            if (current_state.white() & bit_mask3) {
                std::cout << Escapes::YELLOW << "X " << Escapes::COLOR_RESET;
            }
            else if (current_state.black() & bit_mask3) {
                std::cout << Escapes::GREEN << "O " << Escapes::COLOR_RESET;
            }
            else if (current_state.find_moves(current_color) & bit_mask3) {
                std::cout << Escapes::RED << "+ " << Escapes::COLOR_RESET;
            }
            else {
                std::cout << "- ";
            }
            bit_mask3 >>= 1;
        }
        if (i == 1) {
            std::cout << "    WHITE";
        }
        if (i == 2) {
            std::cout << "    " << white_score;
        }
        if (i == 5) {
            std::cout << "    BLACK";
        }
        if (i == 6) {
            std::cout << "    " << black_score;
        }
        std::cout << '\n';
    }
}
