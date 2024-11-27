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

#include "engine/negascout.h"
#include <iostream>
#include <bit>
#include <vector>
#include <thread>

// initialize stats counters and select move order
Negascout::Negascout(Engine::Settings settings) : total_heuristic_count(0), total_state_count(0), move_order(Move_order::Orders::OPTIMIZED) {
    this->settings = settings;
}

uint64_t Negascout::search(Board state, bool color) {
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
    bool first = true;
    Board next;
    
    if (color == true && possible_moves != 0) {
        best_eval = -1000;
        for (uint64_t move : move_order) {
            if (possible_moves & move) {
                next = state;
                next.play_move(color, move);
                
                if (first) { // run first move with whole window
                    eval = negascout(next, settings.search_depth-1, !color, alpha, beta, false);
                    first = false;
                }
                else {
                    eval = negascout(next, settings.search_depth-1, !color, alpha, alpha+1, false); // minimize search window
                    if (eval > alpha && eval < beta) { // if we missed the window and there might still be better move, rerun
                        eval = negascout(next, settings.search_depth-1, !color, eval, beta, false);
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
                next = state;
                next.play_move(color, move);
                
                if (first) { // run first move with whole window
                    eval = negascout(next, settings.search_depth-1, !color, alpha, beta, false);
                    first = false;
                }
                else {
                    eval = negascout(next, settings.search_depth-1, !color, beta-1, beta, false); // minimize search window
                    if (eval < beta && eval > alpha) { // if we missed the window and there might still be better move, rerun
                        eval = negascout(next, settings.search_depth-1, !color, alpha, eval, false);
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

    std::cout << "Went through " << last_state_count     << " states.\n";
    std::cout << "Analyzed     " << last_heuristic_count << " states.\n";
    std::cout << best_eval << '\n';
    total_heuristic_count += last_heuristic_count;
    total_state_count += last_state_count;
    return best_move;
}

int Negascout::negascout(Board state, int depth, bool cur_color, int alpha, int beta, bool end_board) {
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
                next = state;
                next.play_move(cur_color, move);
                
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
        for (uint64_t move : move_order) {
            if (possible_moves & move) {
                next = state;
                next.play_move(cur_color, move);

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
    if (settings.transposition_enable && depth > 2) {
        transposition_table.insert(hash, best_eval, init_alpha, init_beta);
    }

    return best_eval;
}

// initialize stats counters and select move order
NegascoutParallel::NegascoutParallel(Engine::Settings settings) : move_order(Move_order::Orders::OPTIMIZED), manager(settings.thread_count) {
    this->settings = settings;
}

void NegascoutParallel::search_move(void *args) {
    SearchArg *args_ = (SearchArg*) args;
    int eval;
    Board next = args_->state;
    next.play_move(args_->cur_color, args_->move);

    // load latest alpha beta values
    args_->obj->m.lock();
    int alpha_loc = *(args_->alpha);
    int beta_loc = *(args_->beta);
    args_->obj->m.unlock();

    // run the search
    if (args_->cur_color) {
            eval = args_->obj->negascout(next, args_->obj->settings.search_depth-1, !(args_->cur_color), alpha_loc, alpha_loc+1, false); // minimize search window
            if (eval > alpha_loc && eval < beta_loc) { // if we missed the window and there might still be better move, rerun
                eval = args_->obj->negascout(next, args_->obj->settings.search_depth-1, !(args_->cur_color), eval, beta_loc, false);
            }
    }
    else { 
            eval = args_->obj->negascout(next, args_->obj->settings.search_depth-1, !(args_->cur_color), beta_loc-1, beta_loc, false); // minimize search window
            if (eval < beta_loc && eval > alpha_loc) { // if we missed the window and there might still be better move, rerun
                eval = args_->obj->negascout(next, args_->obj->settings.search_depth-1, !(args_->cur_color), alpha_loc, eval, false);
            }
    }

    // update alpha beta values
    args_->obj->m.lock();
    if (args_->cur_color) *(args_->alpha) = std::max(eval, alpha_loc);
    else *(args_->beta) = std::min(eval, beta_loc);
    args_->obj->m.unlock();

    // save search result
    args_->ret = eval;
}

uint64_t NegascoutParallel::search(Board state, bool color) {
    // transposition table must be empty before calculation of best move, otherwise results would be affected
    transposition_table.clear();

    // prepare vectors for holding results from the threads
    uint64_t possible_moves = state.find_moves(color);
    uint64_t possible_moves_count = std::popcount(possible_moves);
    std::vector<SearchArg> evals(possible_moves_count);
    std::vector<uint64_t> moves(possible_moves_count);

    // initialize alpha beta values
    int alpha = -1000;
    int beta = 1000;
    bool first = true;

    int id = 0;
    
    for (uint64_t move : move_order) {
        if (possible_moves & move) {
            // save info about the move
            SearchArg arg = {state, move, color, &alpha, &beta, 0, this};
            moves[id] = move;
            evals[id] = arg;
            // first move does not run in parallel in order to not completely kill pruning performance
            if (first) {
                Board next = state;
                next.play_move(color, move);
                int res = negascout(next, settings.search_depth-1, !color, alpha, beta, false);
                if (color) alpha = res;
                else beta = res;
                evals[id].ret = res;
                first = false;
            }
            // other moves are search in parallel with the help of thread manager
            else {
                manager.add_task(search_move, static_cast<void*>(&(evals[id])));
            }
            id++;
        }
    }

    // wait until all moves are searched
    manager.join();

    int best_eval;
    if (color) best_eval = -1000;
    else best_eval = 1000;

    uint64_t best_move = 0;
    for (size_t i = 0; i < moves.size(); ++i) {
        int eval = evals[i].ret;
        uint64_t move = moves[i];
        if ((color && eval > best_eval) || (!color && eval < best_eval)) {
            best_eval = eval;
            best_move = move;
        }
    }

    std::cout << best_eval << '\n';
    return best_move;
}

int NegascoutParallel::negascout(Board state, int depth, bool cur_color, int alpha, int beta, bool end_board) {
    int init_alpha = alpha;
    int init_beta = beta;
    uint64_t hash = 0;
    
    // reach max depth
    if (depth == 0) {
        return state.rate_board();
    }
    
    // check if state was already calculated
    // overhead of using transposition table becomes
    // too large at lower levels, it is then faster
    // to just calculate the score again
    if (settings.transposition_enable && depth > 2) {
        hash = state.hash();
        int score = transposition_table.get(hash, alpha, beta);
        if (score != TranspositionTableParallel::NOT_FOUND) {
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
                next = state;
                next.play_move(cur_color, move);
                
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
        for (uint64_t move : move_order) {
            if (possible_moves & move) {
                next = state;
                next.play_move(cur_color, move);

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
    if (settings.transposition_enable && depth > 2) {
        transposition_table.insert(hash, best_eval, init_alpha, init_beta);
    }

    return best_eval;
}
