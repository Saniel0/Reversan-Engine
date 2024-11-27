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

#include "ui/terminal.h"
#include <iostream>

Terminal::Terminal(UIStyle style) {
    load_style(style);
    std::cout << Escapes::SWITCH_BUFFER;
    std::cout << Escapes::BOLD << pallete.background;
}

Terminal::Terminal(ColorPallete pallete) {
    this->pallete = pallete;
    std::cout << Escapes::SWITCH_BUFFER;
    std::cout << Escapes::BOLD << pallete.background;
}

Terminal::~Terminal() {
    std::cout << Escapes::RESTORE_BUFFER;
}

std::string Terminal::row_block(int left_size, int block_size, int right_size, bool parity) const {
    // initialize string
    std::string str;
    // change to wanted style
    str += pallete.edge;
    // generate left edge
    for (int i = 0; i < left_size; ++i) {
        str += ' ';
    }
    // generate middle block
    for (int i = 0; i < block_size; ++i) {
        if (i % 2 == parity) str += 'x';
        else str += 'o';
    }
    // generate right edge
    for (int i = 0; i < right_size; ++i) {
        str += ' ';
    }
    // change style back and return the final string
    return str;
}

void Terminal::clear_terminal() const {
    std::cout << Escapes::CLEAR_BUFFER;
}

UI::UserInput Terminal::get_input() {
    // prompt user
    std::cout << Escapes::BLINK << pallete.text << "Select move > " << Escapes::BLINK_RESET;
    // read input
    UserInput input;
    std::cin >> input.x >> input.y;
    return input;
}

void Terminal::wait_for_input() {
    std::cin.get(); // load enter from previous input get
    std::cin.get(); // wait for actual confirm enter
}

void Terminal::display_message(std::string str) {
    std::cout << str << '\n';
}

void Terminal::display_error_message(std::string str) {
    std::cout << Escapes::RESTORE_BUFFER << str << '\n' << Escapes::SWITCH_BUFFER;
}

void Terminal::load_style(UIStyle style) {
    if (style == UIStyle::BASIC) {
        pallete = {
            Escapes::WHITE,
            Escapes::GRAY,
            Escapes::BACK_NONE,
            Escapes::YELLOW,
            Escapes::GREEN,
            Escapes::RED
        };
    }
    else if (style == UIStyle::SOLARIZED) {
        pallete = {
            "\033[38;5;230m",
            "\033[38;5;188m",
            "\e[48;5;237m",
            "\e[38;5;178m",
            "\e[38;5;81m",
            "\e[38;5;166m",
        };
    }
    else {
        pallete = {
            "\033[38;5;38m",
            "\033[38;5;25m",
            "\033[48;5;235m",
            "\033[38;5;46m",
            "\033[38;5;201m",
            "\033[38;5;226m"
        };
    }
}

void Terminal::print_title() const {
    std::cout
        << row_block(0, 57, 0, false) << '\n'
        << row_block(0, 5, 2, true)  << pallete.text << " ____                                     "      << row_block(3, 5, 0, true)  << '\n'
        << row_block(0, 5, 2, false) << pallete.text << "|  _ \\ _____   _____ _ __ ___  __ _ _ __  "     << row_block(3, 5, 0, false) << '\n'
        << row_block(0, 5, 2, true)  << pallete.text <<"| |_) / _ \\ \\ / / _ \\ '__/ __|/ _` | '_ \\ "  << row_block(3, 5, 0, true)  << '\n'
        << row_block(0, 5, 2, false) << pallete.text << "|  _ <  __/\\ V /  __/ |  \\__ \\ (_| | | | |"   << row_block(3, 5, 0, false) << '\n'
        << row_block(0, 5, 2, true)  << pallete.text << "|_| \\_\\___| \\_/ \\___|_|  |___/\\__,_|_| |_|" << row_block(3, 5, 0, true)  << '\n'
        << row_block(0, 5, 24, true) << row_block(23, 5, 0, true) << '\n'
        << row_block(0, 57, 0, false) << '\n';
}

void Terminal::display_board(Board &board, uint64_t moves) {
    uint64_t bit_mask = static_cast<uint64_t>(1) << 63;
    std::cout << pallete.text << "  0 1 2 3 4 5 6 7\n";
    for (int i = 0; i < 8; ++i) {
        std::cout << pallete.text << i << ' ';
        for (int j = 0; j < 8; ++j) {
            if (board.white() & bit_mask) {
                std::cout << pallete.white << "X ";
            }
            else if (board.black() & bit_mask) {
                std::cout << pallete.black << "O ";
            }
            else if (moves & bit_mask) {
                std::cout << pallete.move << "+ ";
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
    // reset view
    clear_terminal();
    print_title();
    std::cout << row_block(0, 5, 24, true) << row_block(23, 5, 0, true) << '\n';
    std::cout << row_block(0, 5, 5, false) << pallete.text << "0 1 2 3 4 5 6 7   " << pallete.background << '|' << pallete.text << "     0 1 2 3 4 5 6 7" << row_block(3, 5, 0, false) << '\n';

    // print boards
    uint64_t bit_mask1 = static_cast<uint64_t>(1) << 63;
    uint64_t bit_mask3 = static_cast<uint64_t>(1) << 63;
    for (int i = 0; i < 8; ++i) {
        std::cout << row_block(0, 5, 3, (i + 1) % 2) << pallete.text << i << ' ';
        for (int j = 0; j < 8; ++j) {
            if (last_state.white() & bit_mask1) {
                std::cout << pallete.white << "X ";
            }
            else if (last_state.black() & bit_mask1) {
                std::cout << pallete.black << "O ";
            }
            
            else if (last_state.find_moves(!current_color) & bit_mask1) {
                std::cout << pallete.move << "+ ";
            }
            else {
                std::cout << pallete.text << "- ";
            }
            bit_mask1 >>= 1;
        }
        std::cout << pallete.text << "  |   " << i << ' ';
        for (int j = 0; j < 8; ++j) {
            if (current_state.white() & bit_mask3) {
                std::cout << pallete.white << "X ";
            }
            else if (current_state.black() & bit_mask3) {
                std::cout << pallete.black << "O ";
            }
            else if (current_state.find_moves(current_color) & bit_mask3) {
                std::cout << pallete.move << "+ ";
            }
            else {
                std::cout << pallete.text << "- ";
            }
            bit_mask3 >>= 1;
        }
        std::cout << row_block(2, 5, 0, (i + 1) % 2) << '\n';
    }

    // display score
    int white_score = current_state.count_white();
    int black_score = current_state.count_black();
    std::cout << row_block(0, 5, 24, true) << row_block(23, 5, 0, true) << '\n';
    std::cout << row_block(0, 57, 0, false) << '\n';
    std::cout << row_block(0, 5, 8 - (white_score >= 10), true) << pallete.white << "X -> " << white_score;
    std::cout << pallete.black << "                   O -> " << black_score << row_block(8 - (black_score >= 10), 5, 0, true) << '\n';
    std::cout << row_block(0, 57, 0, false) << '\n';
}
