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

#ifndef SEARCH_H
#define SEARCH_H

#include <cstdint>
#include "board.h"
#include "move_order.h"
#include "transposition_table.h"

/**
 * @brief Class implementing game-tree search algorithms.
 */
class Search {
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
        Transposition_table transposition_table;
        
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
        int negascout(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board);

        /**
         * @brief Minimax search algorithm with alpha-beta pruning used to find the best move.
         * 
         * @param state A pointer to the current game board state.
         * @param depth The maximum depth of the search tree.
         * @param cur_color The current player's color (true for one color, false for the other).
         * @param alpha The alpha value for alpha-beta pruning.
         * @param beta The beta value for alpha-beta pruning.
         * @param end_board Flag indicating whether the current board state is the final state.
         * @return The evaluated score of the board.
         */
        int minimax(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board);

    public:
        /**
         * @brief Constructor for the Search class. Initializes counters and search algorithms. 
         */
        Search();

        /**
         * @brief Starts the Negascout search from the given board state.
         * 
         * @param state A pointer to the current game board state.
         * @param color The current player's color (true for one color, false for the other).
         * @param search_depth The depth limit for the search algorithm.
         * @return The best move as a bitboard.
         */
        uint64_t start_negascout(Board *state, bool color, int search_depth);

        /**
         * @brief Starts the basic Minimax search from the given board state.
         * 
         * @param state A pointer to the current game board state.
         * @param color The current player's color (true for one color, false for the other).
         * @param search_depth The depth limit for the search algorithm.
         * @return The best move as a bitboard.
         */
        uint64_t start_minimax(Board *state, bool color, int search_depth);
};

#endif
