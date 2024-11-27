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

// __atribute__ is not standard, enable only with supported compillers
#if defined(__GNUC__) || defined(__clang__)
#define ALWAYS_INLINE __attribute__((always_inline))
#else
#define ALWAYS_INLINE
#endif

#include "engine/transposition_table.h"

void TranspositionTable::clear() {
    map.clear();
}

ALWAYS_INLINE void TranspositionTable::insert(uint64_t hash, int score, int alpha, int beta) {
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

ALWAYS_INLINE int TranspositionTable::get(uint64_t hash, int alpha, int beta) {
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

TranspositionTableParallel::TranspositionTableParallel() : maps(map_count), mutexes(map_count) {}

void TranspositionTableParallel::clear() {
    for (auto &m : maps) {
        m.clear();
    }
}

ALWAYS_INLINE void TranspositionTableParallel::insert(uint64_t hash, int score, int alpha, int beta) {
    uint64_t id = hash % map_count;
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
    mutexes[id].lock();
    maps[id][hash] = e;
    mutexes[id].unlock();
}

ALWAYS_INLINE int TranspositionTableParallel::get(uint64_t hash, int alpha, int beta) {
    uint64_t id = hash % map_count;
    mutexes[id].lock();
    if (maps[id].find(hash) != maps[id].end()) {
        Entry e = maps[id][hash];
        mutexes[id].unlock();
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
    else {
        mutexes[id].unlock();
    }
    return NOT_FOUND;
}
