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

void print_title_blur() {
    std::cout << "░▒▓███████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓███████▓▒░ ░▒▓███████▓▒░▒▓█▓▒░\n";
    std::cout << "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░\n";
    std::cout << "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░       ░▒▓█▓▒▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░\n";
    std::cout << "░▒▓███████▓▒░░▒▓██████▓▒░  ░▒▓█▓▒▒▓█▓▒░░▒▓██████▓▒░ ░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░\n";
    std::cout << "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░\n";
    std::cout << "░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░\n";
    std::cout << "░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░  ░▒▓██▓▒░  ░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░░▒▓█▓▒░\n";
}

void print_title_3d() {
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

void print_title_tile() {
    std::cout << "██████  ███████ ██    ██ ███████ ██████  ███████ ██ \n";
    std::cout << "██   ██ ██      ██    ██ ██      ██   ██ ██      ██ \n";
    std::cout << "██████  █████   ██    ██ █████   ██████  ███████ ██ \n";
    std::cout << "██   ██ ██       ██  ██  ██      ██   ██      ██ ██ \n";
    std::cout << "██   ██ ███████   ████   ███████ ██   ██ ███████ ██ \n";
}

void delete_lines(int count) {
    for (int i = 0; i < count; ++i) {
        std::cout << "\033[A\033[K";
    }
}

void print_boards(Board *last_state, Board *current_state, bool current_color) {
    uint64_t bit_mask1 = static_cast<uint64_t>(1) << 63;
    uint64_t bit_mask2 = static_cast<uint64_t>(1) << 63;
    uint64_t bit_mask3 = static_cast<uint64_t>(1) << 63;
    int white_score = __builtin_popcountll(current_state->white_bitmap);
    int black_score = __builtin_popcountll(current_state->black_bitmap);
    std::cout << "\033[1m  0 1 2 3 4 5 6 7        0 1 2 3 4 5 6 7     |       0 1 2 3 4 5 6 7\n\033[0m";
    for (int i = 0; i < 8; ++i) {
        std::cout << "\033[1m" << i << " \033[0m";
        for (int j = 0; j < 8; ++j) {
            if (last_state->white_bitmap & bit_mask1) {
                std::cout << "\033[1;33mX \033[0m";
            }
            else if (last_state->black_bitmap & bit_mask1) {
                std::cout << "\033[1;32mO \033[0m";
            }
            else if (last_state->find_moves(!current_color) & bit_mask1) {
                std::cout << "\033[31m+ \033[0m";
            }
            else {
                std::cout << "- ";
            }
            bit_mask1 >>= 1;
        }
        std::cout << "\033[1m     " << i << " \033[0m";
        for (int j = 0; j < 8; ++j) {
            if (current_state->white_bitmap & bit_mask2) {
                std::cout << "\033[1;33mX \033[0m";
            }
            else if (current_state->black_bitmap & bit_mask2) {
                std::cout << "\033[1;32mO \033[0m";
            }
            else {
                std::cout << "- ";
            }
            bit_mask2 >>= 1;
        }
        std::cout << "\033[1m    |     " << i << " \033[0m";
        for (int j = 0; j < 8; ++j) {
            if (current_state->white_bitmap & bit_mask3) {
                std::cout << "\033[1;33mX \033[0m";
            }
            else if (current_state->black_bitmap & bit_mask3) {
                std::cout << "\033[1;32mO \033[0m";
            }
            else if (current_state->find_moves(current_color) & bit_mask3) {
                std::cout << "\033[31m+ \033[0m";
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