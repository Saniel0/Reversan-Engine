#include "reversi.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <list>

#ifndef NO_SIMD
#include <immintrin.h>
#endif

std::unordered_map<uint64_t, int> transpositionTable;

uint64_t MurmurHash3(uint64_t key) {
    key ^= key >> 33;
    key *= 0xff51afd7ed558ccd;
    key ^= key >> 33;
    key *= 0xc4ceb9fe1a85ec53;
    key ^= key >> 33;
    return key;
}

uint64_t hash_combine(uint64_t a, uint64_t b) {
    uint64_t hash_a = MurmurHash3(a);
    uint64_t hash_b = MurmurHash3(b);
    return hash_a ^ hash_b;
}

uint64_t Reversi::find_best_move(Board *state, bool color, int search_depth) {
    heuristic_count = 0;
    state_count = 0;
    int depth = search_depth;
    uint64_t best_move = 0;
    uint64_t possible_moves = state->find_moves(color);
    int best;

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
                    best = eval;
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
                    best = eval;
                }
                beta = std::min(eval, beta);
            }
        }
        delete next;
    }
    std::cout << "Went through " << state_count     << " states.\n";
    std::cout << "Analyzed     " << heuristic_count << " states.\n";
    std::cout << best << '\n';
    return best_move;
}

int Reversi::minimax(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board) {
    uint64_t hash;
    state_count++;
    // reach max depth
    if (depth == 0) {
        heuristic_count++;
        return state->rate_board();
    }
    if (depth > 1) {
        hash = hash_combine(state->white_bitmap, state->black_bitmap);
        if (transpositionTable.find(hash) != transpositionTable.end()) {
            return transpositionTable[hash];
        }
    }
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
        if (depth > 1) {
            transpositionTable[hash] = max_eval;
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
        if (depth > 1) {
            transpositionTable[hash] = min_eval;
        }
        return min_eval;
    }
}
