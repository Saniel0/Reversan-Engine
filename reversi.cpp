#include "reversi.h"
#include <algorithm>
#include <iostream>
#include <bit>
#include <immintrin.h>

int Reversi::heuristics_map[64] = {100,-15, 10,  5,  5, 10,-15,100,
                                   -15,-30, -2, -2, -2, -2,-30,-15,
                                    10, -2,  1, -1, -1,  1, -2, 10,
                                     5, -2, -1, -1, -1, -1, -2,  5,
                                     5, -2, -1, -1, -1, -1, -2,  5,
                                    10, -2,  1, -1, -1,  1, -2, 10,
                                   -15,-30, -2, -2, -2, -2,-30,-15,
                                   100,-15, 10,  5,  5, 10,-15,100};

[[gnu::noinline]]
int Reversi::heuristics(Board *state, int moves_delta) {
    int score = 0;
    heuristic_count++;
    
#ifdef NO_SIMD
    for (int i = 0; i < 64; ++i) {
        score += ((state->white_bitmap >> i) & 1) * heuristics_map[i];
        score -= ((state->black_bitmap >> i) & 1) * heuristics_map[i];
    }
#else
    // the goal is to manipulate the bitmap to get 256bit vector containing
    // 8bit signed integers, each representing one position on a board
    
    // 8bit * 64 = 512bit, so the board does not fit into
    // one 256bit vector, solved by splitting board in half
    
    uint64_t white_bitmaps1[4]; // first set of vectors, right half of the board
    uint64_t black_bitmaps1[4];
    // shift column we want to proccess into the rightest position (column 7)
    white_bitmaps1[0] = state->white_bitmap; // column 7
    black_bitmaps1[0] = state->black_bitmap;
    white_bitmaps1[1] = (state->white_bitmap >> 1); // column 6
    black_bitmaps1[1] = (state->black_bitmap >> 1);
    white_bitmaps1[2] = (state->white_bitmap >> 2); // column 5
    black_bitmaps1[2] = (state->black_bitmap >> 2);
    white_bitmaps1[3] = (state->white_bitmap >> 3); // column 4
    black_bitmaps1[3] = (state->black_bitmap >> 3);

    uint64_t white_bitmaps2[4]; // second set of vectors, left half of the board
    uint64_t black_bitmaps2[4];
    white_bitmaps2[0] = (state->white_bitmap >> 4); // column 3
    black_bitmaps2[0] = (state->black_bitmap >> 4);
    white_bitmaps2[1] = (state->white_bitmap >> 5); // column 2
    black_bitmaps2[1] = (state->black_bitmap >> 5);
    white_bitmaps2[2] = (state->white_bitmap >> 6); // column 1
    black_bitmaps2[2] = (state->black_bitmap >> 6);
    white_bitmaps2[3] = (state->white_bitmap >> 7); // column 0
    black_bitmaps2[3] = (state->black_bitmap >> 7);

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

    // load data into vector registeres
    __m256i heur_vec1 = _mm256_loadu_si256((__m256i *) heur_map1);
    __m256i heur_vec2 = _mm256_loadu_si256((__m256i *) heur_map2);
    __m256i white_vec1 = _mm256_loadu_si256((__m256i *) white_bitmaps1);
    __m256i white_vec2 = _mm256_loadu_si256((__m256i *) white_bitmaps2);
    __m256i black_vec1 = _mm256_loadu_si256((__m256i *) black_bitmaps1);
    __m256i black_vec2 = _mm256_loadu_si256((__m256i *) black_bitmaps2);

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

    score += 10 * moves_delta;
    return score;
}

uint64_t Reversi::find_best_move(Board *state, bool color) {
    heuristic_count = 0;
    state_count = 0;
    int depth = 10;
    uint64_t best_move = 0;
    uint64_t possible_moves = state->find_moves(color);

    if (color == true && possible_moves != 0) {
        int max_eval = -1000;
        int alpha = -1000;
        int beta = 1000;
        Board *next = new Board();
        int eval;
        for (uint64_t move = static_cast<uint64_t>(1) << 63; move != 0; move >>= 1) {
            if (possible_moves & move) {
                next->copy_state(state);
                next->play_move(color, move);
                eval = minimax(next, depth-1, !color, alpha, beta, false);
                if (eval > max_eval) {
                    best_move = move;
                    max_eval = eval;
                }
                alpha = std::max(eval, alpha);
            }
        }
        delete next;
    }
    else if (color == false && possible_moves != 0) {
        int min_eval = 1000;
        int alpha = -1000;
        int beta = 1000;
        Board *next = new Board();
        int eval;
        for (uint64_t move = static_cast<uint64_t>(1) << 63; move != 0; move >>= 1) {
            if ((possible_moves & move) != 0) {
                next->copy_state(state);
                next->play_move(color, move);
                eval = minimax(next, depth-1, !color, alpha, beta, false);
                if (eval < min_eval) {
                    best_move = move;
                    min_eval = eval;
                }
                beta = std::min(eval, beta);
            }
        }
        delete next;
    }
    std::cout << "Went through " << state_count     << " states.\n";
    std::cout << "Analyzed     " << heuristic_count << " states.\n";
    return best_move;
}

int Reversi::minimax(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board) {
    state_count++;
    // reach max depth
    if (depth == 0) {
        int moves_white = __builtin_popcountll(state->find_moves(true));
        int moves_black = __builtin_popcountll(state->find_moves(false));
        return heuristics(state, (moves_white - moves_black));
    }
    int best_move_id = 0;
    uint64_t possible_moves = state->find_moves(cur_color);
    if (cur_color == true) {
        int max_eval = -1000;
        if (possible_moves == 0) {
            if (end_board) {
                int moves_white = __builtin_popcountll(state->find_moves(true));
                int moves_black = __builtin_popcountll(state->find_moves(false));
                if (moves_white > moves_black) {max_eval = 999;}
                else if (moves_white < moves_black) {max_eval = -999;}
                else {max_eval = 0;}
            }
            else {
                max_eval = minimax(state, depth, !cur_color, alpha, beta, true);
            }
        }
        else {
            Board *next = new Board();
            int eval;
            for (uint64_t move = static_cast<uint64_t>(1) << 63; move != 0; move >>= 1) {
                if (possible_moves & move) {
                    next->copy_state(state);
                    next->play_move(cur_color, move);
                    eval = minimax(next, depth-1, !cur_color, alpha, beta, false);
                    max_eval = std::max(eval, max_eval);
                    alpha = std::max(eval, alpha);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
            delete next;
        }
        return max_eval;
    }
    else {
        int min_eval = 1000;
        if (possible_moves == 0) {
            if (end_board) {
                int moves_white = __builtin_popcountll(state->find_moves(true));
                int moves_black = __builtin_popcountll(state->find_moves(false));
                if (moves_white > moves_black) {min_eval = 999;}
                else if (moves_white < moves_black) {min_eval = -999;}
                else {min_eval = 0;}
            }
            else {
                min_eval = minimax(state, depth, !cur_color, alpha, beta, true);
            }
        }
        else {
            Board *next = new Board();
            int eval;
            for (uint64_t move = static_cast<uint64_t>(1) << 63; move != 0; move >>= 1) {
                if (possible_moves & move) {
                    next->copy_state(state);
                    next->play_move(cur_color, move);
                    eval = minimax(next, depth-1, !cur_color, alpha, beta, false);
                    min_eval = std::min(eval, min_eval);
                    beta = std::min(eval, beta);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }
            delete next;
        }
        return min_eval;
    }
}
