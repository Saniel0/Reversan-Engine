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

#ifndef TERMINAL_H
#define TERMINAL_H

#include "board.h"

/**
 * @brief Prints a blurred title to the terminal.
 * 
 * This function prints a stylized title with a blur effect to the terminal.
 */
void print_title_blur();

/**
 * @brief Prints help message to terminal.
 */
void print_help();

/**
 * @brief Deletes a specified number of lines from the terminal.
 * 
 * @param count The number of lines to delete from the terminal.
 * 
 * This function uses ANSI escape codes to move the cursor up and clear the line.
 */
void delete_lines(int count);

/**
 * @brief Prints board pieces in color to stdout
 * 
 * @param board Board to print out.
 */
void print_board(Board *board);

/**
 * @brief Prints board pieces and moves in color to stdout
 * 
 * @param board Board to print out.
 * @param moves Moves to print out.
 */
void print_board_moves(Board *board, uint64_t moves);

/**
 * @brief Prints moves in color to stdout
 * 
 * @param moves Moves to print out.
 */
void print_moves(uint64_t moves);

/**
 * @brief Prints the previous and current board states side by side.
 * 
 * @param last_state Pointer to the Board object representing the last state.
 * @param current_state Pointer to the Board object representing the current state.
 * @param current_color Boolean indicating the current player's color (true for white, false for black).
 * 
 * This function displays three boards side by side: the previous state, the current state, and the current state with possible moves highlighted.
 */
void print_boards(Board *last_state, Board *current_state, bool current_color);

#endif
