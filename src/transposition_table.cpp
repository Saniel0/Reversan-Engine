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

#include "transposition_table.h"

void Transposition_table::clear() {
    map.clear();
}

__attribute__((always_inline)) void Transposition_table::insert(uint64_t hash, int score, int alpha, int beta) {
    Entry e;
    e.score = score;
    if (score <= alpha) {
        e.type = 2;
    }
    else if (score >= beta) {
        e.type = 1;
    }
    else {
        e.type = 0;
    }
    map[hash] = e;
}

__attribute__((always_inline)) int Transposition_table::get(uint64_t hash, int alpha, int beta) {
    if (map.find(hash) != map.end()) {
        Entry e = map[hash];
        if (e.type == 0) {
            return e.score;
        }
        if (e.type == 1 && e.score >= beta) {
            return beta;
        }
        if (e.type == 2 && e.score <= alpha) {
            return alpha;
        }
    }
    return NOT_FOUND;
}
