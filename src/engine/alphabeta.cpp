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

#include "engine/alphabeta.h"
#include <iostream>

// initialize stats counters and select move order
Alphabeta::Alphabeta(Engine::Settings settings) : total_heuristic_count(0), total_state_count(0), move_order(settings.order) {
    this->settings = settings;
}

uint64_t Alphabeta::search(Board state, bool color) {
    // transposition table must be empty before calculation of best move, otherwise results would be affected
    transposition_table.clear();
    
    // reset stats counters
    last_heuristic_count = 0;
    last_state_count = 0;
    
    uint64_t best_move = 0;
    uint64_t possible_moves = state.find_moves(color);

    int alpha = -1000;
    int beta = 1000;
    int best_eval = 0;
    int eval;
    Board next;
    
    if (color == true && possible_moves != 0) {
        best_eval = -1000;
        for (uint64_t move : move_order) {
            if (possible_moves & move) {
                next = state;
                next.play_move(color, move);
                eval = alphabeta(next, settings.search_depth-1, !color, alpha, beta, false);
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
                next = state;
                next.play_move(color, move);
                eval = alphabeta(next, settings.search_depth-1, !color, alpha, beta, false);
                if (eval < best_eval) {
                    best_move = move;
                    best_eval = eval;
                }
                beta = std::min(eval, beta);
            }
        }
    }

    std::cout << "Went through " << last_state_count     << " states.\n";
    std::cout << "Analyzed     " << last_heuristic_count << " states.\n";
    std::cout << best_eval << '\n';
    total_heuristic_count += last_heuristic_count;
    total_state_count += last_state_count;
    return best_move;
}

int Alphabeta::alphabeta(Board state, int depth, bool cur_color, int alpha, int beta, bool end_board) {
    int init_alpha = alpha;
    int init_beta = beta;
    uint64_t hash = 0;
    last_state_count++;
    
    // reach max depth
    if (depth == 0) {
        last_heuristic_count++;
        return state.rate_board();
    }
    
    // check if state was already calculated
    // overhead of using transposition table becomes
    // too large at lower levels, it is then faster
    // to just calculate the score again
    if (settings.transposition_enable && depth > 2) {
        hash = state.hash();
        int score = transposition_table.get(hash, alpha, beta);
        if (score != TranspositionTable::NOT_FOUND) {
            return score;
        }
    }
    
    // if there are no possible moves
    uint64_t possible_moves = state.find_moves(cur_color);
    int eval;
    if (possible_moves == 0) {
        if (end_board) {
            int count_white = state.count_white();
            int count_black = state.count_black();
            if (count_white > count_black) {eval = 999;}
            else if (count_white < count_black) {eval = -999;}
            else {eval = 0;}
        }
        else {
            eval = alphabeta(state, depth, !cur_color, alpha, beta, true);
        }
        return eval;
    }

    int best_eval;
    Board next;
    if (cur_color == true) {
        best_eval = -1000;
        for (uint64_t move : move_order) {
            if (possible_moves & move) {
                next = state;
                next.play_move(cur_color, move);
                eval = alphabeta(next, depth-1, !cur_color, alpha, beta, false);
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
                next = state;
                next.play_move(cur_color, move);
                eval = alphabeta(next, depth-1, !cur_color, alpha, beta, false);
                best_eval = std::min(eval, best_eval);
                beta = std::min(eval, beta);
                if (beta <= alpha) {
                    break;
                }
            }
        }
    }
    
    // save the score for future
    if (settings.transposition_enable && depth > 2) {
        transposition_table.insert(hash, best_eval, init_alpha, init_beta);
    }
    
    return best_eval;
}
