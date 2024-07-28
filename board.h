#include <cstdint>

class Board {
    private:
        // bit-masks to prevent wrap-around
        // constexpr -> evaluated during compillation -> "encapsulated define"
        static constexpr uint64_t LEFT_COL_MASK = 0xfefefefefefefefe;
        static constexpr uint64_t RIGHT_COL_MASK = 0x7f7f7f7f7f7f7f7f;
        static constexpr uint64_t NO_COL_MASK = 0xffffffffffffffff;
        static constexpr int heuristics_map[64] = {100,-15, 10,  5,  5, 10,-15,100,
                                                   -15,-30, -2, -2, -2, -2,-30,-15,
                                                    10, -2,  1, -1, -1,  1, -2, 10,
                                                     5, -2, -1, -1, -1, -1, -2,  5,
                                                     5, -2, -1, -1, -1, -1, -2,  5,
                                                    10, -2,  1, -1, -1,  1, -2, 10,
                                                   -15,-30, -2, -2, -2, -2,-30,-15,
                                                   100,-15, 10,  5,  5, 10,-15,100};
    
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
        int rate_board();
        uint64_t find_moves(bool color);
        void play_move(bool color, uint64_t move);
        void copy_state(Board *state);
        uint64_t hash();
        
        // functions to load some default boards
        void load_start_state();
        void load_test_state();
        void load_benchmark_state();
};
