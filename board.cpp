#include <iostream>
#include <cstdint>
#include <cstring>
#include <vector>
#include "reversi.h"

Board::Board() {
    white_bitmap = 0;
    black_bitmap = 0;
}

void Board::copy_state(Board *state) {
    white_bitmap = state->white_bitmap;
    black_bitmap = state->black_bitmap;
}

void Board::load_start_state() {
    // or operations just for readability
    // static cast so the number is not simple integer - shifting would go out of range
    white_bitmap = static_cast<uint64_t>(0b00000000) << 56 |
                   static_cast<uint64_t>(0b00000000) << 48 |
                   static_cast<uint64_t>(0b00000000) << 40 |
                   static_cast<uint64_t>(0b00010000) << 32 |
                   static_cast<uint64_t>(0b00001000) << 24 |
                   static_cast<uint64_t>(0b00000000) << 16 |
                   static_cast<uint64_t>(0b00000000) << 8 |
                   static_cast<uint64_t>(0b00000000);
    
    black_bitmap = static_cast<uint64_t>(0b00000000) << 56 |
                   static_cast<uint64_t>(0b00000000) << 48 |
                   static_cast<uint64_t>(0b00000000) << 40 |
                   static_cast<uint64_t>(0b00001000) << 32 |
                   static_cast<uint64_t>(0b00010000) << 24 |
                   static_cast<uint64_t>(0b00000000) << 16 |
                   static_cast<uint64_t>(0b00000000) << 8 |
                   static_cast<uint64_t>(0b00000000);
}

void Board::load_test_state() {
    // random board used to validate functions
    // or operations just for readability
    // static cast so the number is not simple integer - shifting would go out of range
    white_bitmap = static_cast<uint64_t>(0b01000000) << 56 |
                   static_cast<uint64_t>(0b01000000) << 48 |
                   static_cast<uint64_t>(0b00010000) << 40 |
                   static_cast<uint64_t>(0b00001000) << 32 |
                   static_cast<uint64_t>(0b00010100) << 24 |
                   static_cast<uint64_t>(0b00000000) << 16 |
                   static_cast<uint64_t>(0b00000000) << 8 |
                   static_cast<uint64_t>(0b00000000);
    
    black_bitmap = static_cast<uint64_t>(0b00000000) << 56 |
                   static_cast<uint64_t>(0b00000000) << 48 |
                   static_cast<uint64_t>(0b11100000) << 40 |
                   static_cast<uint64_t>(0b00010000) << 32 |
                   static_cast<uint64_t>(0b00001000) << 24 |
                   static_cast<uint64_t>(0b00000100) << 16 |
                   static_cast<uint64_t>(0b00000000) << 8 |
                   static_cast<uint64_t>(0b00000000);
}

void Board::load_benchmark_state() {
    // one of the more difficult board states to compute
    // or operations just for readability
    // static cast so the number is not simple integer - shifting would go out of range
    white_bitmap = static_cast<uint64_t>(0b00000001) << 56 |
                   static_cast<uint64_t>(0b00010010) << 48 |
                   static_cast<uint64_t>(0b01111110) << 40 |
                   static_cast<uint64_t>(0b01111100) << 32 |
                   static_cast<uint64_t>(0b00111100) << 24 |
                   static_cast<uint64_t>(0b00010100) << 16 |
                   static_cast<uint64_t>(0b00110000) << 8 |
                   static_cast<uint64_t>(0b00010000);
    
    black_bitmap = static_cast<uint64_t>(0b11111100) << 56 |
                   static_cast<uint64_t>(0b00101100) << 48 |
                   static_cast<uint64_t>(0b00000000) << 40 |
                   static_cast<uint64_t>(0b00000000) << 32 |
                   static_cast<uint64_t>(0b11000000) << 24 |
                   static_cast<uint64_t>(0b11101000) << 16 |
                   static_cast<uint64_t>(0b00000000) << 8 |
                   static_cast<uint64_t>(0b00000000);
}

uint64_t Board::find_moves(bool color, int &moves_count) {
    uint64_t valid_moves = 0;
    // create new bitmap of empty spaces from our two bitmaps so we do not have to check both for empty spaces
    uint64_t free_spaces = ~(white_bitmap | black_bitmap);
    // load table of player at turn and opponent player
    uint64_t playing, opponent;
    if (color) {
        playing = white_bitmap;
        opponent = black_bitmap;
    }
    else {
        playing = black_bitmap;
        opponent = white_bitmap;
    }

    // horizontal
    uint64_t dir_copy;
    uint64_t opponent_adjusted = opponent & RIGHT_COL_MASK & LEFT_COL_MASK;
    dir_copy = ((playing << 1) | (playing >> 1)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 1) | (dir_copy >> 1)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 1) | (dir_copy >> 1)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 1) | (dir_copy >> 1)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 1) | (dir_copy >> 1)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 1) | (dir_copy >> 1)) & opponent_adjusted;
    valid_moves |= (dir_copy << 1) | (dir_copy >> 1);
    // diagonal from bottom left to top right
    opponent_adjusted = opponent & RIGHT_COL_MASK & LEFT_COL_MASK;
    dir_copy = ((playing << 7) | (playing >> 7)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 7) | (dir_copy >> 7)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 7) | (dir_copy >> 7)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 7) | (dir_copy >> 7)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 7) | (dir_copy >> 7)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 7) | (dir_copy >> 7)) & opponent_adjusted;
    valid_moves |= (dir_copy << 7) | (dir_copy >> 7);
    // vertical
    dir_copy = ((playing << 8) | (playing >> 8)) & opponent;
    dir_copy |= ((dir_copy << 8) | (dir_copy >> 8)) & opponent;
    dir_copy |= ((dir_copy << 8) | (dir_copy >> 8)) & opponent;
    dir_copy |= ((dir_copy << 8) | (dir_copy >> 8)) & opponent;
    dir_copy |= ((dir_copy << 8) | (dir_copy >> 8)) & opponent;
    dir_copy |= ((dir_copy << 8) | (dir_copy >> 8)) & opponent;
    valid_moves |= (dir_copy << 8) | (dir_copy >> 8);
    // diagonal from bottom right to top left
    opponent_adjusted = opponent & RIGHT_COL_MASK & LEFT_COL_MASK;
    dir_copy = ((playing << 9) | (playing >> 9)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 9) | (dir_copy >> 9)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 9) | (dir_copy >> 9)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 9) | (dir_copy >> 9)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 9) | (dir_copy >> 9)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 9) | (dir_copy >> 9)) & opponent_adjusted;
    valid_moves |= (dir_copy << 9) | (dir_copy >> 9);
    // mask by free spaces to get the result
    valid_moves &= free_spaces;

    // valid moves are returned in form of bitmap
    return valid_moves;
}

void Board::play_move(bool color, uint64_t move) {
    uint64_t playing, opponent;
    if (color) {
        playing = white_bitmap;
        opponent = black_bitmap;
    }
    else {
        playing = black_bitmap;
        opponent = white_bitmap;
    }

    auto check_dir = [&](uint64_t col_mask, int shift) {
        bool found = false;
        uint64_t playing_adjusted = playing & col_mask;
        uint64_t opponent_adjusted = opponent & col_mask;
        uint64_t offset = (shift < 0) ? (move << (-shift)) : (move >> shift);
        uint64_t line = 0;
        while (opponent_adjusted & offset) {
            found = true;
            line |= offset;
            offset = (shift < 0) ? (offset << (-shift)) : (offset >> shift);
        }
        if (found && (playing_adjusted & offset)) {
            playing |= line;
            opponent ^= line;
        }
    };

    playing |= move; // capture the space
    check_dir(LEFT_COL_MASK ,-9); // top left
    check_dir(NO_COL_MASK   ,-8); // top
    check_dir(RIGHT_COL_MASK,-7); // top right
    check_dir(LEFT_COL_MASK ,-1); // left
    check_dir(RIGHT_COL_MASK, 1); // right
    check_dir(LEFT_COL_MASK , 7); // bottom left
    check_dir(NO_COL_MASK   , 8); // bottom
    check_dir(RIGHT_COL_MASK, 9); // bottom right

    if (color) {
        white_bitmap = playing;
        black_bitmap = opponent;
    }
    else {
        white_bitmap = opponent;
        black_bitmap = playing;
    }
}

void Board::print_board() {
    uint64_t bit_mask = static_cast<uint64_t>(1) << 63;
    std::cout << "\033[1m  0 1 2 3 4 5 6 7\n\033[0m";
    for (int i = 0; i < 8; ++i) {
        std::cout << "\033[1m" << i << " \033[0m";
        for (int j = 0; j < 8; ++j) {
            if (white_bitmap & bit_mask) {
                std::cout << "\033[1;33mX \033[0m";
            }
            else if (black_bitmap & bit_mask) {
                std::cout << "\033[1;32mO \033[0m";
            }
            else {
                std::cout << "- ";
            }
            bit_mask >>= 1;
        }
        std::cout << '\n';
    }
}

void Board::print_board_moves(uint64_t moves) {
    uint64_t bit_mask = static_cast<uint64_t>(1) << 63;
    std::cout << "\033[1m  0 1 2 3 4 5 6 7\n\033[0m";
    for (int i = 0; i < 8; ++i) {
        std::cout << "\033[1m" << i << " \033[0m";
        for (int j = 0; j < 8; ++j) {
            if (white_bitmap & bit_mask) {
                std::cout << "\033[1;33mX \033[0m";
            }
            else if (black_bitmap & bit_mask) {
                std::cout << "\033[1;32mO \033[0m";
            }
            else if (moves & bit_mask) {
                std::cout << "\033[31m+ \033[0m";
            }
            else {
                std::cout << "- ";
            }
            bit_mask >>= 1;
        }
        std::cout << '\n';
    }
}

void Board::print_moves(uint64_t moves) {
    uint64_t bit_mask = static_cast<uint64_t>(1) << 63;
    std::cout << "\033[1m  0 1 2 3 4 5 6 7\n\033[0m";
    for (int i = 0; i < 8; ++i) {
        std::cout << "\033[1m" << i << " \033[0m";
        for (int j = 0; j < 8; ++j) {
            if (moves & bit_mask) {
                std::cout << "\033[1;31mX \033[0m";
            }
            else {
                std::cout << "- ";
            }
            bit_mask >>= 1;
        }
        std::cout << '\n';
    }
}
