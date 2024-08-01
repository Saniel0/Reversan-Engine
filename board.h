#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

/**
 * @brief Class representing a game board using bitmaps.
 * 
 * The Board class uses two 64-bit integers to represent the positions of pieces
 * on a game board. Each bit in the bitmap represents a space on the board.
 * Unless specified otherwise by NO_SIMD, the class uses AVX2 SIMD instructions
 * to achieve the best performance.
 */
class Board {
    private:
        /**
         * @brief Bit masks to prevent wrap-around during bitwise operations.
         * 
         * These masks are used to ensure that bitwise operations do not cause
         * pieces to wrap around to the other side of the board.
         */
        static constexpr uint64_t LEFT_COL_MASK = 0xfefefefefefefefe; /**< Mask to prevent left column wrap-around. */
        static constexpr uint64_t RIGHT_COL_MASK = 0x7f7f7f7f7f7f7f7f; /**< Mask to prevent right column wrap-around. */
        static constexpr uint64_t NO_COL_MASK = 0xffffffffffffffff; /**< Mask with all bits set (no wrap-around). */
        
        /**
         * @brief Heuristic values for board evaluation.
         * 
         * The heuristics_map array contains predefined values used to evaluate
         * the board state. The index represents the position on the board, and
         * the value represents the heuristic score for that position.
         */
        static constexpr int heuristics_map[64] = {100,-15, 10,  5,  5, 10,-15,100,
                                                   -15,-30, -2, -2, -2, -2,-30,-15,
                                                    10, -2,  1, -1, -1,  1, -2, 10,
                                                     5, -2, -1, -1, -1, -1, -2,  5,
                                                     5, -2, -1, -1, -1, -1, -2,  5,
                                                    10, -2,  1, -1, -1,  1, -2, 10,
                                                   -15,-30, -2, -2, -2, -2,-30,-15,
                                                   100,-15, 10,  5,  5, 10,-15,100
        };
    
    public:
        /**
         * @brief Bitmaps representing the positions of pieces.
         * 
         * Each bit represents a space on the board, where a set bit indicates
         * the presence of a piece.
         */
        uint64_t white_bitmap; /**< Bitmap representing white_pieces. */
        uint64_t black_bitmap; /**< Bitmap representing black_pieces. */

        /**
         * @brief Constructor to initialize the board bitmaps.
         * 
         * Initializes the bitmaps to 0 to ensure memory safety.
         */
        Board();

        /**
         * @brief Copies the state of the board.
         * 
         * @param state Pointer to the Board object from which the state will be copied.
         */
        void copy_state(Board *state);

        /**
         * @brief Finds all possible moves for the given color.
         * 
         * @param color Boolean indicating the color (true for white, false for black).
         * @return uint64_t Bitmap representing all possible moves for the given color.
         */
        uint64_t find_moves(bool color);

        /**
         * @brief Plays a move on the board.
         * 
         * @param color Boolean indicating the color (true for white, false for black).
         * @param move Bitmap representing the move to be played.
         * 
         * Updates the board state by playing the specified move.
         */
        void play_move(bool color, uint64_t move);

        /**
         * @brief Rates the current board state.
         * 
         * @return int The heuristic score of the current board state.
         * 
         * Evaluates the board using the heuristic values and other methods to determine a score.
         */
        int rate_board();

        /**
         * @brief Generates a hash value for the current board state.
         * 
         * @return uint64_t The hash value representing the current board state.
         * 
         * Uses murmur3 hashing algorithm.
         */
        uint64_t hash();
        
        /**
         * @brief Functions for loading specific board states
         * 
         */
        void load_start_state(); /**< Loads the start state based on official rules. */
        void load_benchmark_state(); /**< Loads custom state for comparing performance. */
        void load_test_state(); /**< Special custom state used to test correctness of the algorithm. */

        /**
         * @brief Functions for printing out board states to stdout
         * 
         */
        void print_board();
        void print_board_moves(uint64_t moves);
        void print_moves(uint64_t moves);
};

#endif
