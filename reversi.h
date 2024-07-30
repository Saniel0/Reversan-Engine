#include <cstdint>
#include "board.h"

class Reversi {
    private:
        // used only for statistics
        unsigned long long int heuristic_count;
        unsigned long long int state_count;
        
        // heuristics for board evaluation
        static int heuristics_map[64];
        
        int negascout(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board);
        int minimax(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board);
        //int heuristics(Board *state, int moves_delta, bool end_board);

    public:
        uint64_t find_best_move(Board *state, bool color, int search_depth);
};
