#include <cstdint>

class Board {
    private:
        // bit-masks to prevent wrap-around
        // constexpr -> evaluated during compillation -> "encapsulated define"
        static constexpr uint64_t LEFT_COL_MASK = 0xfefefefefefefefe;
        static constexpr uint64_t RIGHT_COL_MASK = 0x7f7f7f7f7f7f7f7f;
        static constexpr uint64_t NO_COL_MASK = 0xffffffffffffffff;
    
    public:
        // board is represented with bitmaps, each bit represents one space on board
        uint64_t white_bitmap;
        uint64_t black_bitmap;

        // initializes bitmaps for memory safety
        Board();
        
        // prints board state to stdout
        void print_board();
        void print_board_moves(uint64_t moves);
        void print_moves(uint64_t moves);
        uint64_t find_moves(bool color, int &moves_count);
        void play_move(bool color, uint64_t move);
        void copy_state(Board *state);
        
        // functions to load some default boards
        void load_start_state();
        void load_test_state();
        void load_benchmark_state();
};

class Reversi {
    private:
        // used only for statistics
        unsigned long long int heuristic_count;
        unsigned long long int state_count;
        
        // heuristics for board evaluation
        static int heuristics_map[64];
        
        int minimax(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board);
        int heuristics(Board *state, int moves_delta, bool end_board);

    public:
        uint64_t find_best_move(Board *state, bool color);

};
