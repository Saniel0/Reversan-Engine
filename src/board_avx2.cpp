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

#include "board.h"
#include <immintrin.h>

// heuristics map has to be converted into format optimized for SIMD instructions
constexpr uint64_t convert_col(int col) {
    uint64_t val = 0;
    for (int i = 0; i < 8; ++i) {
        val <<= 8;
        val |= static_cast<uint8_t>(Board::heuristics_map[i*8 + col]);
    }
    return val;
}

static constexpr uint64_t heur_map1[4] = {convert_col(7), convert_col(6), convert_col(5), convert_col(4)};
static constexpr uint64_t heur_map2[4] = {convert_col(3), convert_col(2), convert_col(1), convert_col(0)};


__attribute__((always_inline)) int Board::rate_board() {
    int score = 0;
    
    // the goal is to manipulate the bitmap to get 256bit vector containing
    // 8bit signed integers, each representing one position on a board
    
    // 8bit * 64 = 512bit, so the board does not fit into
    // one 256bit vector, solved by splitting board in half

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
    
    int moves_delta = __builtin_popcountll(find_moves(true)) - __builtin_popcountll(find_moves(false));
    score += 10 * moves_delta;
    return score;
}

__attribute__((always_inline)) uint64_t Board::find_moves(bool color) {
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

    // same algorithm as non-vectorized code, uses SIMD
    // to proccess all directions at once
    //
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
    
    // opponent vec must be masked to prevent wrap around
    __m256i col_mask_vec = _mm256_set_epi64x(SIDE_COLS_MASK, NO_COL_MASK, SIDE_COLS_MASK, SIDE_COLS_MASK);
    __m256i opponent_vec = _mm256_set1_epi64x(opponent);
    __m256i opponent_adjusted_vec = _mm256_and_si256(opponent_vec, col_mask_vec);

    // ans vec is at start filled with 4 bitboards of player at turn
    __m256i ans_vec = _mm256_set1_epi64x(playing);
    
    // FIRST SHIFT
    __m256i left_shift_vec = _mm256_sllv_epi64(ans_vec, shift_vals_vec);
    __m256i right_shift_vec = _mm256_srlv_epi64(ans_vec, shift_vals_vec);
    // compute first iteration
    __m256i tmp_and_vec;
    __m256i tmp_or_vec = _mm256_or_si256(left_shift_vec, right_shift_vec);
    ans_vec = _mm256_and_si256(tmp_or_vec, opponent_adjusted_vec);

    // loop through the rest of iterations (5 remaining)
    for (int i = 0; i < 5; ++i) {
        // shift
        left_shift_vec = _mm256_sllv_epi64(ans_vec, shift_vals_vec);
        right_shift_vec = _mm256_srlv_epi64(ans_vec, shift_vals_vec);
        // compute iteration
        tmp_or_vec = _mm256_or_si256(left_shift_vec, right_shift_vec);
        tmp_and_vec = _mm256_and_si256(tmp_or_vec, opponent_adjusted_vec);
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

    // mask by free spaces to get the result
    valid_moves &= free_spaces;
    // valid moves are returned in form of bitmap
    return valid_moves;
}

__attribute__((always_inline)) void Board::play_move(bool color, uint64_t move) {
    uint64_t playing, opponent;
    if (color) {
        playing = white_bitmap;
        opponent = black_bitmap;
    }
    else {
        playing = black_bitmap;
        opponent = white_bitmap;
    }

    // 9 -> top left / bottom right
    // 8 -> up / down
    // 7 -> top right / bottom left
    // 1 -> left / right
    __m256i shift_vals_vec = _mm256_set_epi64x(9, 8, 7, 1);

    __m256i col_mask_vec = _mm256_set_epi64x(SIDE_COLS_MASK, NO_COL_MASK, SIDE_COLS_MASK, SIDE_COLS_MASK);
    __m256i compare_vec = _mm256_set1_epi64x(0);
    __m256i move_vec = _mm256_set1_epi64x(move);

    // load bitboards into vectors, 4 in each
    __m256i playing_vec = _mm256_set1_epi64x(playing);
    __m256i opponent_vec = _mm256_set1_epi64x(opponent);
    __m256i opponent_adjusted_vec =_mm256_and_si256(opponent_vec, col_mask_vec); // prevent wrap around

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

    if (color) {
        white_bitmap = playing;
        black_bitmap = opponent;
    }
    else {
        white_bitmap = opponent;
        black_bitmap = playing;
    }
}
