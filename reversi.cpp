#include "reversi.h"
#include <algorithm>
#include <iostream>
#include <bit>

int Reversi::heuristics_map[64] = {100,-15, 10,  5,  5, 10,-15,100,
                                   -15,-30, -2, -2, -2, -2,-30,-15,
                                    10, -2,  1, -1, -1,  1, -2, 10,
                                     5, -2, -1, -1, -1, -1, -2,  5,
                                     5, -2, -1, -1, -1, -1, -2,  5,
                                    10, -2,  1, -1, -1,  1, -2, 10,
                                   -15,-30, -2, -2, -2, -2,-30,-15,
                                   100,-15, 10,  5,  5, 10,-15,100};

int Reversi::heuristics(Board *state, int moves_delta, bool end_board) {
    int score = 0;
    heuristic_count++;
    if (end_board) {
        for (uint64_t bitmask = static_cast<uint64_t>(1) << 63; bitmask != 0; bitmask >>= 1) {
            if (state->white_bitmap & bitmask) {
                score++;
            }
            else if (state->black_bitmap & bitmask) {
                score--;
            }
        }
        if (score > 0) {
            score = 999;
        }
        else if (score < 0) {
            score = -999;
        }
        else {
            score = 0;
        }
    }
    else {
        for (int i = 0; i < 64; ++i) {
            score += ((state->white_bitmap >> i) & 1) * heuristics_map[i];
            score -= ((state->black_bitmap >> i) & 1) * heuristics_map[i];         
        }
        score += 10 * moves_delta;
    }
    return score;
} 

uint64_t Reversi::find_best_move(Board *state, bool color) {
    heuristic_count = 0;
    state_count = 0;
    int depth = 10;
    int moves_count = 0;
    uint64_t best_move = 0;
    uint64_t possible_moves = state->find_moves(color, moves_count);
    moves_count = __builtin_popcountll(possible_moves);

    if (color == true && moves_count != 0) {
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
    else if (color == false && moves_count != 0) {
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
        int moves_white = 0;
        int moves_black = 0;
        uint64_t white_moves = state->find_moves(true, moves_white);
        uint64_t black_moves = state->find_moves(false, moves_black);
        moves_white = __builtin_popcountll(white_moves);
        moves_black = __builtin_popcountll(black_moves);
        return heuristics(state, (moves_white - moves_black), false);
    }
    //search_count++;
    int moves_count = 0;
    int best_move_id = 0;
    uint64_t possible_moves = state->find_moves(cur_color, moves_count);
    moves_count = __builtin_popcountll(possible_moves);
    if (cur_color == true) {
        int max_eval = -1000;
        if (moves_count == 0) {
            if (end_board) {
                max_eval = heuristics(state, moves_count, true);
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
        if (moves_count == 0) {
            if (end_board) {
                min_eval = heuristics(state, moves_count, true);
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
