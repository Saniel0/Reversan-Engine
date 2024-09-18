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

#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

/**
 * @brief Class representing a game board using bitmaps.
 * 
 * The Board class uses two 64-bit integers to represent the positions of pieces
 * on a game board. Each bit in the bitmap represents a space on the board.
 */
class Board {
    private:
        /**
         * @brief Bit mask.
         * 
         * Used to ensure that bitwise operations do not cause
         * pieces to wrap around to the other side of the board.
         */
        static constexpr uint64_t LEFT_COL_MASK = 0xfefefefefefefefe;

        /**
         * @brief Bit mask.
         * 
         * Used to ensure that bitwise operations do not cause
         * pieces to wrap around to the other side of the board.
         */
        static constexpr uint64_t RIGHT_COL_MASK = 0x7f7f7f7f7f7f7f7f;

        /**
         * @brief Bit mask.
         * 
         * Used to ensure that bitwise operations do not cause
         * pieces to wrap around to the other side of the board.
         */
        static constexpr uint64_t SIDE_COLS_MASK = 0x7e7e7e7e7e7e7e7e;

        /**
         * @brief Bit mask.
         * 
         * Used to ensure that bitwise operations do not cause
         * pieces to wrap around to the other side of the board.
         */
        static constexpr uint64_t NO_COL_MASK = 0xffffffffffffffff;

    public:
        
        /**
         * @brief Heuristic values for board evaluation.
         * 
         * Value at every position HAS TO BE between -127 128,
         * otherwise AVX2 version breaks.
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
    
        /**
         * @brief Bitmap representing the positions of white pieces.
         * 
         * Each bit represents a space on the board, where a set bit indicates
         * the presence of a piece.
         */
        uint64_t white_bitmap;

        /**
         * @brief Bitmap representing the positions of black pieces.
         * 
         * Each bit represents a space on the board, where a set bit indicates
         * the presence of a piece.
         */
        uint64_t black_bitmap;

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
         * @brief Function initializing start state.
         */
        void load_start_state();

        /**
         * @brief Function initializing benchmark state.
         */
        void load_benchmark_state();

        /**
         * @brief Function initializing test state.
         */
        void load_test_state();

        /**
         * @brief Functions that prints board pieces to stdout
         */
        void print_board();

        /**
         * @brief Functions that prints board pieces and moves to stdout
         * 
         * @param moves Moves to print out along with board pieces.
         */
        void print_board_moves(uint64_t moves);

        /**
         * @brief Functions that prints moves to stdout
         * 
         * @param moves Moves to print out.
         */
        void print_moves(uint64_t moves);
};

#endif
