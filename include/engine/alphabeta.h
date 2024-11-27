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

#ifndef ALPHABETA_H
#define ALPHABETA_H

#include "engine/engine.h"
#include "engine/move_order.h"
#include "engine/transposition_table.h"

/**
 * @brief Class implementing negascout game-tree search.
 */
class Alphabeta : public Engine {
    private:
        /// @brief Number of heuristic evaluations performed in the last search (used for statistics).
        unsigned long long int last_heuristic_count;

        /// @brief Number of game states evaluated in the last search (used for statistics).
        unsigned long long int last_state_count;

        /// @brief Number of heuristic evaluations performed in the lifetime of class instance (used for statistics).
        unsigned long long int total_heuristic_count;

        /// @brief Number of game states evaluated in the lifetime of class instance (used for statistics).
        unsigned long long int total_state_count;
        
        /// @brief Array storing the order in which possible moves are evaluated to optimize search performance.
        Move_order move_order;

        /// @brief The transposition table used to store previously evaluated game states and their results, improving search efficiency.
        TranspositionTable transposition_table;

        /**
         * @brief Negascout search algorithm (a variant of alpha-beta pruning) used to find the best move.
         * 
         * @param state A pointer to the current game board state.
         * @param depth The maximum depth of the search tree.
         * @param cur_color The current player's color (true for one color, false for the other).
         * @param alpha The alpha value for alpha-beta pruning.
         * @param beta The beta value for alpha-beta pruning.
         * @param end_board Flag indicating whether the current board state is the final state.
         * @return The evaluated score of the board.
         */
        int alphabeta(Board state, int depth, bool cur_color, int alpha, int beta, bool end_board);

    public:
        /// @brief Constructor initializing settings. 
        explicit Alphabeta(Engine::Settings settings);
        
        uint64_t search(Board state, bool color) override;
};

#endif