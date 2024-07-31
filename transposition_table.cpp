#include "transposition_table.h"

void Transposition_table::clear() {
    map.clear();
}

void Transposition_table::insert(uint64_t hash, int score, int alpha, int beta) {
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

int Transposition_table::get(uint64_t hash, int alpha, int beta) {
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
