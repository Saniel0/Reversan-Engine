#ifndef TRANSPOSITION_H
#define TRANSPOSITION_H

#include <cstdint>
#include <unordered_map>

#define NOT_FOUND 1111

class Transposition_table {
    private:
        struct Entry {
            int score;
            int type;
        };
        std::unordered_map<uint64_t, Entry> map;

    public:
        void clear();
        void insert(uint64_t hash, int score, int alpha, int beta);
        int get(uint64_t hash, int alpha, int beta);
};

#endif
