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

#ifndef ENGINE_H
#define ENGINE_H

#include "board/board.h"
#include "move_order.h"

/**
 * @brief Class implementing game-tree search algorithms.
 */
class Engine {
    public:
        /// @brief Engine search settings.
        struct Settings {
            int search_depth;
            int time_limit;
            int thread_count;
            bool transposition_enable;
            const uint8_t *order;
        };

        /// @brief List of avaible algorithms.
        enum class Alg {
            ALPHABETA,
            NEGASCOUT
        };

        /// @brief Virtual deconstructor to ensure all derived classes can deleted properly.
        virtual ~Engine() {};

        /**
         * @brief Starts the search from the given board state.
         * 
         * @param state A pointer to the current game board state.
         * @param color The current player's color (true for one color, false for the other).
         * 
         * @return The best move as a bitboard.
         */
        virtual uint64_t search(Board state, bool color) = 0;

    protected:
        /// @brief Loaded search settings.
        Settings settings;
};

#endif
