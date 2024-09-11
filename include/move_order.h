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

#ifndef MOVE_ORDER_H
#define MOVE_ORDER_H

#include <cstdint>

/**
 * @brief Class containing an array specifying order of move evaluation.
 * 
 * Order of checking possible moves has huge impact on performance.
 * It is crucial to optimize the move order. Changin the move order
 * can affect result, if two possible moves have the same state score.
 */
class Move_order {
    private:
        /// @brief Internal array for storing the loaded order
        uint64_t move_order[64];

    public:
        /// @brief Simple example order, very bad pruning performance, not reccomended.
        static constexpr int LINE_BY_LINE[64] = {  0,  1,  2,  3,  4,  5,  6,  7,
                                                   8,  9, 10, 11, 12, 13, 14, 15,
                                                  16, 17, 18, 19, 20, 21, 22, 23,
                                                  24, 25, 26, 27, 28, 29, 30, 31,
                                                  32, 33, 34, 35, 36, 37, 38, 39,
                                                  40, 41, 42, 43, 44, 45, 46, 47,
                                                  48, 49, 50, 51, 52, 53, 54, 55,
                                                  56, 57, 58, 59, 60, 61, 62, 63
        };

        /// @brief Optimized order based on board heuristics values.
        static constexpr int OPTIMIZED[64] = {  0, 20,  4, 12, 13,  5, 21,  1,
                                               22, 28, 29, 30, 31, 32, 33, 23,
                                                6, 34, 35, 36, 37, 38, 39,  7,
                                               14, 40, 41, 42, 43, 44, 45, 15,
                                               16, 46, 47, 48, 49, 50, 51, 17,
                                                8, 52, 53, 54, 55, 56, 57,  9,
                                               24, 58, 59, 60, 61, 62, 63, 25,
                                                2, 26, 10, 18, 19, 11, 27,  3
        };

        /// @brief Alternate optimized order based on board heuristics values.
        static constexpr int OPTIMIZED2[64] = {  0, 52,  4, 12, 13,  5, 53,  1,
                                               54, 60, 36, 37, 38, 39, 61, 55,
                                                6, 40, 20, 24, 25, 21, 41,  7,
                                               14, 42, 26, 27, 28, 29, 43, 15,
                                               16, 44, 30, 31, 32, 33, 45, 17,
                                                8, 46, 22, 34, 35, 23, 47,  9,
                                               56, 62, 48, 49, 50, 51, 63, 57,
                                                2, 58, 10, 18, 19, 11, 59,  3
        };

        /**
         * @brief Constructor for the Move_order class.
         * 
         * @param order Order in which to check possible moves.
         */
        Move_order(const int *order);

        /// @brief Begin iterator (points to the first element of the array)
        uint64_t* begin() {
            return move_order;
        }

        /// @brief End iterator (points to the one-past-the-last element)
        uint64_t* end() {
            return move_order + 64;
        }
};

#endif
