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

#include "move_order.h"
#include <iostream>
#include <cstring>

Move_order::Move_order(const int *order) {
    // check order validity, if it is not valid, initialize array to zeros
    if (order != LINE_BY_LINE && order != OPTIMIZED && order != OPTIMIZED2) {
        std::cerr << "Invalid move order! Initializing move order to zeros." << '\n';
        memset(move_order, 0, 64 * sizeof(uint64_t));
        return;
    }

    // if valid order was provided, load it
    uint64_t cur = 1L << 63;
    for (int i = 0; i < 64; ++i) {
        move_order[order[i]] = cur;
        cur >>= 1;
    }
}
