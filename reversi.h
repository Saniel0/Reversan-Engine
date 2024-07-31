#ifndef REVERSI_H
#define REVERSI_H

#include <cstdint>
#include "board.h"
#include "transposition_table.h"

class Reversi {
    private:
        // used only for statistics
        unsigned long long int heuristic_count;
        unsigned long long int state_count;
        
        // order in which possible moves are checked, important for performance reasons
        uint64_t move_order[64];

        Transposition_table transposition_table;
        
        int negascout(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board);
        int minimax(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board);
        //int heuristics(Board *state, int moves_delta, bool end_board);

    public:
        Reversi();
        uint64_t start_negascout(Board *state, bool color, int search_depth);
        uint64_t start_minimax(Board *state, bool color, int search_depth);
};

#endif
