#include "reversi.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>

#ifndef NO_SIMD
#include <immintrin.h>
#endif

#define NONE 1111

struct SSS {
    int score;
    int type;
};

std::unordered_map<uint64_t, SSS> transpositionTable;

int retrieve_score(uint64_t hash, int alpha, int beta) {
    if (transpositionTable.find(hash) != transpositionTable.end()) {
        SSS s = transpositionTable[hash];
        if (s.type == 0) {
            return s.score;
        }
        if (s.type == 1 && s.score >= beta) {
            return beta;
        }
        if (s.type == 2 && s.score <= alpha) {
            return alpha;
        }
    }
    return NONE;
}

void save_score(uint64_t hash, int score, int alpha, int beta) {
    SSS s;
    s.score = score;
    if (score <= alpha) {
        s.type = 2;
    }
    else if (score >= beta) {
        s.type = 1;
    }
    else {
        s.type = 0;
    }
    transpositionTable[hash] = s;
}

uint64_t Reversi::start_negascout(Board *state, bool color, int depth) {
    transpositionTable.clear();
    heuristic_count = 0;
    state_count = 0;
    uint64_t best_move = 0;
    uint64_t possible_moves = state->find_moves(color);

    int alpha = -1000;
    int beta = 1000;
    int eval;
    int best_eval;
    bool first = true;
    Board *next = new Board();
    
    if (color == true && possible_moves != 0) {
        best_eval = -1000;
        for (uint64_t move = static_cast<uint64_t>(1) << 63; move != 0; move >>= 1) {
            if (possible_moves & move) {
                next->copy_state(state);
                next->play_move(color, move);
                
                if (first) { // run first move with whole window
                    eval = negascout(next, depth-1, !color, alpha, beta, false);
                    first = false;
                }
                else {
                    eval = negascout(next, depth-1, !color, alpha, alpha+1, false); // minimize search window
                    if (eval > alpha && eval < beta) { // if we missed the window and there might still be better move, rerun
                        eval = negascout(next, depth-1, !color, eval, beta, false);
                    }
                }

                if (eval > best_eval) {
                    best_move = move;
                    best_eval = eval;
                }
                alpha = std::max(eval, alpha);
            }
        }
    }
    else if (color == false && possible_moves != 0) {
        best_eval = 1000;
        for (uint64_t move = static_cast<uint64_t>(1) << 63; move != 0; move >>= 1) {
            if ((possible_moves & move) != 0) {
                next->copy_state(state);
                next->play_move(color, move);
                
                if (first) { // run first move with whole window
                    eval = negascout(next, depth-1, !color, alpha, beta, false);
                    first = false;
                }
                else {
                    eval = negascout(next, depth-1, !color, beta-1, beta, false); // minimize search window
                    if (eval < beta && eval > alpha) { // if we missed the window and there might still be better move, rerun
                        eval = negascout(next, depth-1, !color, alpha, eval, false);
                    }
                }

                if (eval < best_eval) {
                    best_move = move;
                    best_eval = eval;
                }
                beta = std::min(eval, beta);
            }
        }
    }
    delete next;

    std::cout << "Went through " << state_count     << " states.\n";
    std::cout << "Analyzed     " << heuristic_count << " states.\n";
    std::cout << best_eval << '\n';
    return best_move;
}

int Reversi::negascout(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board) {
    int init_alpha = alpha;
    int init_beta = beta;
    uint64_t hash = 0;
    state_count++;
    
    // reach max depth
    if (depth == 0) {
        heuristic_count++;
        return state->rate_board();
    }
    
    // check if state was already calculated
    if (depth > 2) {
        hash = state->hash();
        int score = retrieve_score(hash, alpha, beta);
        if (score != NONE) {
            return score;
        }
    }

    // if there are no possible moves
    uint64_t possible_moves = state->find_moves(cur_color);
    int eval;
    if (possible_moves == 0) {
        if (end_board) {
            int moves_white = __builtin_popcountll(state->find_moves(true));
            int moves_black = __builtin_popcountll(state->find_moves(false));
            if (moves_white > moves_black) {eval = 999;}
            else if (moves_white < moves_black) {eval = -999;}
            else {eval = 0;}
        }
        else {
            eval = negascout(state, depth, !cur_color, alpha, beta, true);
        }
        return eval;
    }

    int best_eval;
    bool first = true;
    Board *next = new Board();
    if (cur_color == true) {
        best_eval = -1000;
        for (uint64_t move = static_cast<uint64_t>(1) << 63; move != 0; move >>= 1) {
            if (possible_moves & move) {
                next->copy_state(state);
                next->play_move(cur_color, move);
                
                if (first) { // run first move with whole window
                    eval = negascout(next, depth-1, !cur_color, alpha, beta, false);
                    first = false;
                }
                else {
                    eval = negascout(next, depth-1, !cur_color, alpha, alpha+1, false); // minimize search window
                    if (eval > alpha && eval < beta) { // if we missed the window and there might still be better move, rerun
                        eval = negascout(next, depth-1, !cur_color, eval, beta, false);
                    }
                }

                best_eval = std::max(eval, best_eval);
                alpha = std::max(eval, alpha);
                if (beta <= alpha) {
                    break;
                }
            }
        }
    }
    else {
        best_eval = 1000;
        for (uint64_t move = static_cast<uint64_t>(1) << 63; move != 0; move >>= 1) {
            if (possible_moves & move) {
                next->copy_state(state);
                next->play_move(cur_color, move);

                if (first) { // run first move with whole window
                    eval = negascout(next, depth-1, !cur_color, alpha, beta, false);
                    first = false;
                }
                else {
                    eval = negascout(next, depth-1, !cur_color, beta-1, beta, false); // minimize search window
                    if (eval < beta && eval > alpha) { // if we missed the window and there might still be better move, rerun
                        eval = negascout(next, depth-1, !cur_color, alpha, eval, false);
                    }
                }
                
                best_eval = std::min(eval, best_eval);
                beta = std::min(eval, beta);
                if (beta <= alpha) {
                    break;
                }
            }
        }
    }
    
    // save the score for future
    if (depth > 2) {
        save_score(hash, best_eval, init_alpha, init_beta);
    }

    delete next;
    return best_eval;
}

uint64_t Reversi::start_minimax(Board *state, bool color, int depth) {
    transpositionTable.clear();
    heuristic_count = 0;
    state_count = 0;
    uint64_t best_move = 0;
    uint64_t possible_moves = state->find_moves(color);

    int alpha = -1000;
    int beta = 1000;
    int eval;
    int best_eval;
    Board *next = new Board();
    
    if (color == true && possible_moves != 0) {
        best_eval = -1000;
        for (uint64_t move = static_cast<uint64_t>(1) << 63; move != 0; move >>= 1) {
            if (possible_moves & move) {
                next->copy_state(state);
                next->play_move(color, move);
                eval = minimax(next, depth-1, !color, alpha, beta, false);
                if (eval > best_eval) {
                    best_move = move;
                    best_eval = eval;
                }
                alpha = std::max(eval, alpha);
            }
        }
    }
    else if (color == false && possible_moves != 0) {
        best_eval = 1000;
        for (uint64_t move = static_cast<uint64_t>(1) << 63; move != 0; move >>= 1) {
            if ((possible_moves & move) != 0) {
                next->copy_state(state);
                next->play_move(color, move);
                eval = minimax(next, depth-1, !color, alpha, beta, false);
                if (eval < best_eval) {
                    best_move = move;
                    best_eval = eval;
                }
                beta = std::min(eval, beta);
            }
        }
    }
    delete next;

    std::cout << "Went through " << state_count     << " states.\n";
    std::cout << "Analyzed     " << heuristic_count << " states.\n";
    std::cout << best_eval << '\n';
    return best_move;
}

int Reversi::minimax(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board) {
    int init_alpha = alpha;
    int init_beta = beta;
    uint64_t hash = 0;
    state_count++;
    
    // reach max depth
    if (depth == 0) {
        heuristic_count++;
        return state->rate_board();
    }
    
    // check if state was already calculated
    if (depth > 2) {
        hash = state->hash();
        int score = retrieve_score(hash, alpha, beta);
        if (score != NONE) {
            return score;
        }
    }
    
    // if there are no possible moves
    uint64_t possible_moves = state->find_moves(cur_color);
    int eval;
    if (possible_moves == 0) {
        if (end_board) {
            int moves_white = __builtin_popcountll(state->find_moves(true));
            int moves_black = __builtin_popcountll(state->find_moves(false));
            if (moves_white > moves_black) {eval = 999;}
            else if (moves_white < moves_black) {eval = -999;}
            else {eval = 0;}
        }
        else {
            eval = minimax(state, depth, !cur_color, alpha, beta, true);
        }
        return eval;
    }

    int best_eval;
    Board *next = new Board();
    if (cur_color == true) {
        best_eval = -1000;
        for (uint64_t move = static_cast<uint64_t>(1) << 63; move != 0; move >>= 1) {
            if (possible_moves & move) {
                next->copy_state(state);
                next->play_move(cur_color, move);
                eval = minimax(next, depth-1, !cur_color, alpha, beta, false);
                best_eval = std::max(eval, best_eval);
                alpha = std::max(eval, alpha);
                if (beta <= alpha) {
                    break;
                }
            }
        }
    }
    else {
        best_eval = 1000;
        for (uint64_t move = static_cast<uint64_t>(1) << 63; move != 0; move >>= 1) {
            if (possible_moves & move) {
                next->copy_state(state);
                next->play_move(cur_color, move);
                eval = minimax(next, depth-1, !cur_color, alpha, beta, false);
                best_eval = std::min(eval, best_eval);
                beta = std::min(eval, beta);
                if (beta <= alpha) {
                    break;
                }
            }
        }
    }
    
    // save the score for future
    if (depth > 2) {
        save_score(hash, best_eval, init_alpha, init_beta);
    }
    
    delete next;
    return best_eval;
}
