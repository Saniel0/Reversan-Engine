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

    You should have received a copy of the GNU General Public License along with Foobar. If not, see <https://www.gnu.org/licenses/>. 
*/

#include "board.h"
#include <iostream>

#ifndef NO_SIMD
#include <immintrin.h>
#endif

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

int Board::rate_board() {
    int score = 0;
    
#ifdef NO_SIMD
    for (int i = 0; i < 64; ++i) {
        score += ((white_bitmap >> i) & 1) * heuristics_map[i];
        score -= ((black_bitmap >> i) & 1) * heuristics_map[i];
    }
#else
    // the goal is to manipulate the bitmap to get 256bit vector containing
    // 8bit signed integers, each representing one position on a board
    
    // 8bit * 64 = 512bit, so the board does not fit into
    // one 256bit vector, solved by splitting board in half

    // load heuristic matrix values into corresponding positions
    int64_t heur_map1[4];
    heur_map1[0] = 0x64f10a05050af164; // column 7
    heur_map1[1] = 0xf1e2fefefefee2f1; // column 6
    heur_map1[2] = 0x0afe01ffff01fe0a; // column 5
    heur_map1[3] = 0x05fefffffffffe05; // column 4

    int64_t heur_map2[4];
    heur_map2[0] = 0x05fefffffffffe05; // column 3
    heur_map2[1] = 0x0afe01ffff01fe0a; // column 2
    heur_map2[2] = 0xf1e2fefefefee2f1; // column 1
    heur_map2[3] = 0x64f10a05050af164; // column 0

    // specific values to shift each column into the rightest position
    __m256i shift_vals_vec1 = _mm256_set_epi64x(3, 2, 1, 0);
    __m256i shift_vals_vec2 = _mm256_set_epi64x(7, 6, 5, 4);

    // load data into vector registeres
    // 1 -> first set of vectors, right half of the board
    // 2 -> second set of vectors, left half of the board
    __m256i heur_vec1 = _mm256_loadu_si256((__m256i *) heur_map1);
    __m256i heur_vec2 = _mm256_loadu_si256((__m256i *) heur_map2);
    __m256i white_vec1 = _mm256_set1_epi64x(white_bitmap);
    __m256i white_vec2 = _mm256_set1_epi64x(white_bitmap);
    __m256i black_vec1 = _mm256_set1_epi64x(black_bitmap);
    __m256i black_vec2 = _mm256_set1_epi64x(black_bitmap);

    white_vec1 = _mm256_srlv_epi64(white_vec1, shift_vals_vec1);
    white_vec2 = _mm256_srlv_epi64(white_vec2, shift_vals_vec2);
    black_vec1 = _mm256_srlv_epi64(black_vec1, shift_vals_vec1);
    black_vec2 = _mm256_srlv_epi64(black_vec2, shift_vals_vec2);

    // mask out everything except the right column 
    __m256i mask_vec = _mm256_set1_epi64x(0x0101010101010101);
    white_vec1 = _mm256_and_si256(white_vec1, mask_vec);
    white_vec2 = _mm256_and_si256(white_vec2, mask_vec);
    black_vec1 = _mm256_and_si256(black_vec1, mask_vec);
    black_vec2 = _mm256_and_si256(black_vec2, mask_vec);

    // at this point white_vec1/white_vec2/black_vec1/black_ve2
    // are either 0b00000000 or 0b00000001, which is used in multiplication
    // with heuristic values to get the score efficiently

    // multiply board vectors with heuristics vectors and partially add the results together
    // multiply two corresponding 8bit signed integers
    __m256i result_white1 = _mm256_maddubs_epi16(white_vec1, heur_vec1);
    __m256i result_white2 = _mm256_maddubs_epi16(white_vec2, heur_vec2);
    __m256i result_black1 = _mm256_maddubs_epi16(black_vec1, heur_vec1);
    __m256i result_black2 = _mm256_maddubs_epi16(black_vec2, heur_vec2);

    // join the results of the two halves
    __m256i result_white_joined = _mm256_add_epi16(result_white1, result_white2);
    __m256i result_black_joined = _mm256_add_epi16(result_black1, result_black2);

    // subtract black score from white score
    __m256i result_vec = _mm256_sub_epi16(result_white_joined, result_black_joined);

    // create data_out array
    int16_t int_result[16];
    _mm256_storeu_si256((__m256i *) int_result, result_vec); // extract vector result into the array

    // sum all 16bit parts
    for (int i = 0; i < 16; ++i) {
        score += static_cast<int>(int_result[i]);
    }
#endif
    
    int moves_delta = __builtin_popcountll(find_moves(true)) - __builtin_popcountll(find_moves(false));
    score += 10 * moves_delta;
    return score;
}

uint64_t Board::find_moves(bool color) {
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

#ifdef NO_SIMD
    uint64_t dir_copy;
    uint64_t opponent_adjusted = opponent & RIGHT_COL_MASK & LEFT_COL_MASK;
    // horizontal
    dir_copy = ((playing << 1) | (playing >> 1)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 1) | (dir_copy >> 1)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 1) | (dir_copy >> 1)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 1) | (dir_copy >> 1)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 1) | (dir_copy >> 1)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 1) | (dir_copy >> 1)) & opponent_adjusted;
    valid_moves |= (dir_copy << 1) | (dir_copy >> 1);
    // diagonal from bottom left to top right
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
    dir_copy = ((playing << 9) | (playing >> 9)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 9) | (dir_copy >> 9)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 9) | (dir_copy >> 9)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 9) | (dir_copy >> 9)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 9) | (dir_copy >> 9)) & opponent_adjusted;
    dir_copy |= ((dir_copy << 9) | (dir_copy >> 9)) & opponent_adjusted;
    valid_moves |= (dir_copy << 9) | (dir_copy >> 9);
#else
    // same algorithm as non-vectorized code, uses SIMD
    // to proccess all directions at once

    // indexes correspond to certain directions:
    // 0 -> horizontal
    // 1 -> diagonal from bottom left to top right
    // 2 -> vertical
    // 3 -> diagonal from bottom right to top left

    // 9 -> top left / bottom right
    // 8 -> up / down
    // 7 -> top right / bottom left
    // 1 -> left / right
    __m256i shift_vals_vec = _mm256_set_epi64x(9, 8, 7, 1);
    
    // adjust opponent bitboards to handle wrap-around
    uint64_t opponent_adjusted[4];
    opponent_adjusted[0] = opponent & RIGHT_COL_MASK & LEFT_COL_MASK;
    opponent_adjusted[1] = opponent & RIGHT_COL_MASK & LEFT_COL_MASK;
    opponent_adjusted[2] = opponent;
    opponent_adjusted[3] = opponent & RIGHT_COL_MASK & LEFT_COL_MASK;
    __m256i mask_vec = _mm256_loadu_si256((__m256i *) opponent_adjusted);

    // ans vector is at start filled with 4 bitboards of player at turn
    __m256i ans_vec = _mm256_set1_epi64x(playing);
    
    // FIRST SHIFT
    __m256i left_shift_vec = _mm256_sllv_epi64(ans_vec, shift_vals_vec);
    __m256i right_shift_vec = _mm256_srlv_epi64(ans_vec, shift_vals_vec);
    // compute first iteration
    __m256i tmp_and_vec;
    __m256i tmp_or_vec = _mm256_or_si256(left_shift_vec, right_shift_vec);
    ans_vec = _mm256_and_si256(tmp_or_vec, mask_vec);

    // loop through the rest of iterations (5 remaining)
    for (int i = 0; i < 5; ++i) {
        // shift
        left_shift_vec = _mm256_sllv_epi64(ans_vec, shift_vals_vec);
        right_shift_vec = _mm256_srlv_epi64(ans_vec, shift_vals_vec);
        // compute iteration
        tmp_or_vec = _mm256_or_si256(left_shift_vec, right_shift_vec);
        tmp_and_vec = _mm256_and_si256(tmp_or_vec, mask_vec);
        ans_vec = _mm256_or_si256(ans_vec, tmp_and_vec);
    }
    
    // last shift iteration
    left_shift_vec = _mm256_sllv_epi64(ans_vec, shift_vals_vec);
    right_shift_vec = _mm256_srlv_epi64(ans_vec, shift_vals_vec);
    // add valid moves from each directios
    tmp_or_vec = _mm256_or_si256(left_shift_vec, right_shift_vec);
    // extract valid_moves data
    uint64_t valid_moves_data[4];
    _mm256_storeu_si256((__m256i *) valid_moves_data, tmp_or_vec);
    // add result from all directions together
    valid_moves = valid_moves_data[0] | valid_moves_data[1] | valid_moves_data[2] | valid_moves_data[3];
#endif 
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

#ifdef NO_SIMD
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
    check_dir(RIGHT_COL_MASK, 9); // bottom right*/
#else
    // 9 -> top left / bottom right
    // 8 -> up / down
    // 7 -> top right / bottom left
    // 1 -> left / right
    __m256i shift_vals_vec = _mm256_set_epi64x(9, 8, 7, 1);

    // adjust opponent bitboards to handle wrap-around
    uint64_t opponent_adjusted[4];
    opponent_adjusted[0] = opponent & RIGHT_COL_MASK & LEFT_COL_MASK;
    opponent_adjusted[1] = opponent & RIGHT_COL_MASK & LEFT_COL_MASK;
    opponent_adjusted[2] = opponent;
    opponent_adjusted[3] = opponent & RIGHT_COL_MASK & LEFT_COL_MASK;
    
    __m256i opponent_adjusted_vec = _mm256_loadu_si256((__m256i *) opponent_adjusted);
    __m256i opponent_vec = _mm256_set1_epi64x(opponent);
    __m256i playing_vec = _mm256_set1_epi64x(playing);
    __m256i compare_vec = _mm256_set1_epi64x(0);
    __m256i move_vec = _mm256_set1_epi64x(move);

    // FIRST SHIFT
    __m256i left_shift_vec = _mm256_sllv_epi64(move_vec, shift_vals_vec);
    __m256i right_shift_vec = _mm256_srlv_epi64(move_vec, shift_vals_vec);
    // compute
    left_shift_vec = _mm256_and_si256(left_shift_vec, opponent_adjusted_vec);
    right_shift_vec = _mm256_and_si256(right_shift_vec, opponent_adjusted_vec);

    __m256i friendly_left_check = _mm256_set1_epi64x(0);
    __m256i friendly_right_check = _mm256_set1_epi64x(0);
    __m256i left_shift_vec_next;
    __m256i right_shift_vec_next;
    
    for (int i = 0; i < 6; ++i) {
        // shift
        left_shift_vec_next = _mm256_sllv_epi64(left_shift_vec, shift_vals_vec);
        right_shift_vec_next =_mm256_srlv_epi64(right_shift_vec, shift_vals_vec);
        // compute
        left_shift_vec = _mm256_or_si256(left_shift_vec, left_shift_vec_next);
        right_shift_vec = _mm256_or_si256(right_shift_vec, right_shift_vec_next);
        left_shift_vec = _mm256_and_si256(left_shift_vec, opponent_adjusted_vec);
        right_shift_vec = _mm256_and_si256(right_shift_vec, opponent_adjusted_vec);
    }

    friendly_left_check = _mm256_and_si256(left_shift_vec_next, playing_vec);
    friendly_right_check = _mm256_and_si256(right_shift_vec_next, playing_vec);
    friendly_left_check = _mm256_cmpeq_epi64(friendly_left_check, compare_vec);
    friendly_right_check = _mm256_cmpeq_epi64(friendly_right_check, compare_vec);
    __m256i capture_left_vec = _mm256_andnot_si256(friendly_left_check, left_shift_vec);
    __m256i capture_right_vec = _mm256_andnot_si256(friendly_right_check, right_shift_vec);
    opponent_vec = _mm256_xor_si256(opponent_vec, capture_left_vec);
    opponent_vec = _mm256_xor_si256(opponent_vec, capture_right_vec);
    playing_vec = _mm256_or_si256(playing_vec, capture_left_vec);
    playing_vec = _mm256_or_si256(playing_vec, capture_right_vec);

    uint64_t playing_data[4];
    uint64_t opponent_data[4];
    _mm256_storeu_si256((__m256i *) playing_data, playing_vec);
    _mm256_storeu_si256((__m256i *) opponent_data, opponent_vec);
    playing = playing_data[0] | playing_data[1] | playing_data[2] | playing_data[3] | move;
    opponent = opponent_data[0] & opponent_data[1] & opponent_data[2] & opponent_data[3];
#endif

    if (color) {
        white_bitmap = playing;
        black_bitmap = opponent;
    }
    else {
        white_bitmap = opponent;
        black_bitmap = playing;
    }
}

uint64_t Board::hash() {
    // Murmur hash of white
    uint64_t w = white_bitmap;
    w ^= w >> 33;
    w *= 0xff51afd7ed558ccd;
    w ^= w >> 33;
    w *= 0xc4ceb9fe1a85ec53;
    w ^= w >> 33;
    // Murmur hash of black
    uint64_t b = black_bitmap;
    b ^= b >> 33;
    b *= 0xff51afd7ed558ccd;
    b ^= b >> 33;
    b *= 0xc4ceb9fe1a85ec53;
    b ^= b >> 33;
    // Combine white and black hashes
    return w ^ b;
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
