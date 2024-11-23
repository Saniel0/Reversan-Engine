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

#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include <cstdint>
#include <unordered_map>

/**
 * @brief Class representing a transposition table for storing game states.
 * 
 * The Transposition_table class is used to store and retrieve game states
 * identified by their unique hash and alpha-beta values. It uses an internal
 * unordered map to maintain the mapping from hash values to their corresponding
 * entries.
 */
class Transposition_table {
    private:    
        /// @brief Structure representing an entry in the transposition table.
        struct Entry {
            /// @brief The score associated with the game state.
            int score;
            /// @brief The type of the entry (exact, lower bound, upper bound).
            int type;
        };

        /// @brief The internal map storing hash-entry pairs.
        std::unordered_map<uint64_t, Entry> map;

    public:
        /// @brief Constant representing that entry was not found.
        static constexpr int NOT_FOUND = 1111;

        /// @brief Removes all entries stored in the transposition table.
        void clear();

        /**
         * @brief Inserts a new entry into the transposition table.
         * 
         * @param hash The unique hash value identifying the game state.
         * @param score The score associated with the game state.
         * @param alpha The alpha value for alpha-beta pruning.
         * @param beta The beta value for alpha-beta pruning.
         * 
         * Score, alpha and beta values are used to determine entry type.
         */
        void insert(uint64_t hash, int score, int alpha, int beta);

        /**
         * @brief Retrieves an entry from the transposition table.
         * 
         * @param hash The unique hash value identifying the game state.
         * @param alpha The alpha value for alpha-beta pruning.
         * @param beta The beta value for alpha-beta pruning.
         * @return int The score associated with the game state, or NOT_FOUND if the entry is not found.
         * 
         * This method retrieves the score of the game state identified by the
         * given hash value. If the entry is not found, it returns NOT_FOUND.
         */
        int get(uint64_t hash, int alpha, int beta);
};

#endif
