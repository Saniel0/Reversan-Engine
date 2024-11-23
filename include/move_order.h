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
#include <vector>

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
        /// @brief Collection of multiple pre-defined move orders
        struct Orders {
            /// @brief Simple example order, very bad pruning performance, not reccomended.
            static const Move_order LINE_BY_LINE;

            /// @brief Optimized order based on board heuristics values.
            static const Move_order OPTIMIZED;

            /// @brief Alternate optimized order based on board heuristics values.
            static const Move_order OPTIMIZED2;
        };

        /**
         * @brief Default constructor for the Move_order class.
         * 
         * Loads OPTIMIZED move order.
         */
        Move_order();

        /**
         * @brief Constructor for the Move_order class.
         * 
         * @param order Order in form of vector.
         */
        Move_order(const std::vector<uint8_t>& order);

        /// @brief Begin iterator (points to the first element of the array)
        const uint64_t* begin() const {
            return move_order;
        }

        /// @brief End iterator (points to the one-past-the-last element)
        const uint64_t* end() const {
            return move_order + 64;
        }
};

#endif
