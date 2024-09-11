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

#include "search.h"
#include <iostream>

// specify move order in the constructor
Search::Search(): move_order(Move_order::OPTIMIZED){}

uint64_t Search::start_negascout(Board *state, bool color, int depth) {
    transposition_table.clear();
    heuristic_count = 0;
    state_count = 0;
    uint64_t best_move = 0;
    uint64_t possible_moves = state->find_moves(color);

    int alpha = -1000;
    int beta = 1000;
    int best_eval = 0;
    int eval;
    bool first = true;
    Board next;
    
    if (color == true && possible_moves != 0) {
        best_eval = -1000;
        for (uint64_t move : move_order) {
            if (possible_moves & move) {
                next.copy_state(state);
                next.play_move(color, move);
                
                if (first) { // run first move with whole window
                    eval = negascout(&next, depth-1, !color, alpha, beta, false);
                    first = false;
                }
                else {
                    eval = negascout(&next, depth-1, !color, alpha, alpha+1, false); // minimize search window
                    if (eval > alpha && eval < beta) { // if we missed the window and there might still be better move, rerun
                        eval = negascout(&next, depth-1, !color, eval, beta, false);
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
        for (uint64_t move : move_order) {
            if ((possible_moves & move) != 0) {
                next.copy_state(state);
                next.play_move(color, move);
                
                if (first) { // run first move with whole window
                    eval = negascout(&next, depth-1, !color, alpha, beta, false);
                    first = false;
                }
                else {
                    eval = negascout(&next, depth-1, !color, beta-1, beta, false); // minimize search window
                    if (eval < beta && eval > alpha) { // if we missed the window and there might still be better move, rerun
                        eval = negascout(&next, depth-1, !color, alpha, eval, false);
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

    std::cout << "Went through " << state_count     << " states.\n";
    std::cout << "Analyzed     " << heuristic_count << " states.\n";
    std::cout << best_eval << '\n';
    //std::cout << state->rate_board() << '\n';
    //state->print_moves(best_move);
    //std::cout << '\n';
    return best_move;
}

int Search::negascout(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board) {
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
        int score = transposition_table.get(hash, alpha, beta);
        if (score != NOT_FOUND) {
            return score;
        }
    }

    // if there are no possible moves
    uint64_t possible_moves = state->find_moves(cur_color);
    int eval;
    if (possible_moves == 0) {
        if (end_board) {
            int moves_white = __builtin_popcountll(state->white_bitmap);
            int moves_black = __builtin_popcountll(state->black_bitmap);
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
    Board next;
    if (cur_color == true) {
        best_eval = -1000;
        for (uint64_t move : move_order) {
            if (possible_moves & move) {
                next.copy_state(state);
                next.play_move(cur_color, move);
                
                if (first) { // run first move with whole window
                    eval = negascout(&next, depth-1, !cur_color, alpha, beta, false);
                    first = false;
                }
                else {
                    eval = negascout(&next, depth-1, !cur_color, alpha, alpha+1, false); // minimize search window
                    if (eval > alpha && eval < beta) { // if we missed the window and there might still be better move, rerun
                        eval = negascout(&next, depth-1, !cur_color, eval, beta, false);
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
        for (uint64_t move : move_order) {
            if (possible_moves & move) {
                next.copy_state(state);
                next.play_move(cur_color, move);

                if (first) { // run first move with whole window
                    eval = negascout(&next, depth-1, !cur_color, alpha, beta, false);
                    first = false;
                }
                else {
                    eval = negascout(&next, depth-1, !cur_color, beta-1, beta, false); // minimize search window
                    if (eval < beta && eval > alpha) { // if we missed the window and there might still be better move, rerun
                        eval = negascout(&next, depth-1, !cur_color, alpha, eval, false);
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
        transposition_table.insert(hash, best_eval, init_alpha, init_beta);
    }

    return best_eval;
}

uint64_t Search::start_minimax(Board *state, bool color, int depth) {
    transposition_table.clear();
    heuristic_count = 0;
    state_count = 0;
    uint64_t best_move = 0;
    uint64_t possible_moves = state->find_moves(color);

    int alpha = -1000;
    int beta = 1000;
    int best_eval = 0;
    int eval;
    Board next;
    
    if (color == true && possible_moves != 0) {
        best_eval = -1000;
        for (uint64_t move : move_order) {
            if (possible_moves & move) {
                next.copy_state(state);
                next.play_move(color, move);
                eval = minimax(&next, depth-1, !color, alpha, beta, false);
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
        for (uint64_t move : move_order) {
            if ((possible_moves & move) != 0) {
                next.copy_state(state);
                next.play_move(color, move);
                eval = minimax(&next, depth-1, !color, alpha, beta, false);
                if (eval < best_eval) {
                    best_move = move;
                    best_eval = eval;
                }
                beta = std::min(eval, beta);
            }
        }
    }

    std::cout << "Went through " << state_count     << " states.\n";
    std::cout << "Analyzed     " << heuristic_count << " states.\n";
    std::cout << best_eval << '\n';
    return best_move;
}

int Search::minimax(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board) {
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
        int score = transposition_table.get(hash, alpha, beta);
        if (score != NOT_FOUND) {
            return score;
        }
    }
    
    // if there are no possible moves
    uint64_t possible_moves = state->find_moves(cur_color);
    int eval;
    if (possible_moves == 0) {
        if (end_board) {
            int moves_white = __builtin_popcountll(state->white_bitmap);
            int moves_black = __builtin_popcountll(state->black_bitmap);
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
    Board next;
    if (cur_color == true) {
        best_eval = -1000;
        for (uint64_t move : move_order) {
            if (possible_moves & move) {
                next.copy_state(state);
                next.play_move(cur_color, move);
                eval = minimax(&next, depth-1, !cur_color, alpha, beta, false);
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
        for (uint64_t move : move_order) {
            if (possible_moves & move) {
                next.copy_state(state);
                next.play_move(cur_color, move);
                eval = minimax(&next, depth-1, !cur_color, alpha, beta, false);
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
        transposition_table.insert(hash, best_eval, init_alpha, init_beta);
    }
    
    return best_eval;
}
