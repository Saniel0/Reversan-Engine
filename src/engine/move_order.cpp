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

#include "engine/move_order.h"
#include <iostream>

Move_order::Move_order() : Move_order(Orders::OPTIMIZED) {}

Move_order::Move_order(const uint8_t *order) {
    if (order != Orders::LINE_BY_LINE && order != Orders::OPTIMIZED && order != Orders::OPTIMIZED2) {
        std::cerr << "For loading custom orders, use vectors! Initializing default order" << '\n';
        Move_order();
    }
    else {
        uint64_t cur = 1L << 63;
        for (int i = 0; i < 64; ++i) {
            move_order[order[i]] = cur;
            cur >>= 1;
        }
    }
}

Move_order::Move_order(const std::vector<uint8_t>& order) {
    // check order validity, if it is not valid, use default constructor instead
    if (order.size() != 64) {
        std::cerr << "Invalid move order! Initializing default order" << '\n';
        Move_order();
    }
    else {
        uint64_t cur = 1L << 63;
        for (int i = 0; i < 64; ++i) {
            move_order[order[i]] = cur;
            cur >>= 1;
        }
    }
}
