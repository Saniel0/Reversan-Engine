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

#include "ui.h"

class Terminal : public UI {
    private:
        /// @brief Collection of ANSI escape sequences
        struct Escapes {
            static constexpr std::string SWITCH_BUFFER = "\x1B[?1049h";
            static constexpr std::string RESTORE_BUFFER = "\x1B[?1049l";
            static constexpr std::string CLEAR_BUFFER = "\x1B[2J\x1B[H";
            static constexpr std::string COLOR_RESET = "\033[0m";
            static constexpr std::string RED = "\033[1;31m"; // also bold
            static constexpr std::string YELLOW = "\033[1;33m"; // also bold
            static constexpr std::string GREEN = "\033[1;32m"; // also bold
            static constexpr std::string GRAY = "\033[1;90m"; // also bold
            static constexpr std::string BOLD = "\033[1m";
        };

        /**
         * @brief Generates string containg 'count' chars c.
         * 
         * @param count String length.
         * @param c Char to dupe.
         * 
         * @return Generated string.
         */
        std::string dup_string(int count, char c) const;

        /**
         * @brief Generates string for the left edge of the UI.
         * 
         * @param edge_size Size of edge.
         * @param gap_size Size of gap to the right of the edge.
         * @param style ASCII style of the edge.
         * @param parity Parity of the first char of the edge.
         * 
         * @return Generated string.
         */
        std::string left_edge(int edge_size, int gap_size, std::string style, bool parity) const;

        /**
         * @brief Generates string for the right edge of the UI.
         * 
         * @param edge_size Size of edge.
         * @param gap_size Size of gap to the left of the edge.
         * @param style ASCII style of the edge.
         * @param parity Parity of the first char of the edge.
         * 
         * @return Generated string.
         */
        std::string right_edge(int edge_size, int gap_size, std::string style, bool parity) const;

        /**
         * @brief Generates string for one row of the UI.
         * 
         * @param row_size Size of the row - width.
         * @param style ASCII style of the row.
         * @param parity Parity of the first char of the row.
         * 
         * @return Generated string.
         */
        std::string row(int row_size, std::string style, bool parity) const;

        /**
         * @brief Generates string for one row of the UI including gap.
         * 
         * @param row_size Size of the row - width.
         * @param gap_size Size of the middle gap.
         * @param style ASCII style of edges.
         * @param parity Parity of the first char of edges.
         * 
         * @return Generated string.
         */
        std::string row_with_gap(int edge_size, int gap_size, std::string style, bool parity) const;

        /// @brief Clears whole terminal
        void clear_terminal() const;

        /// @brief Prints title to the terminal
        void print_title() const;

    public:
        /// @brief Switches to separate terminal buffer.
        Terminal();
        
        /// @brief Restores original terminal buffer.
        ~Terminal();

        /// @return user input from terminal
        UserInput get_input() override;

        /// @brief Prints help message to the terminal.
        void display_help() override;

        /// @brief Prints custom message.
        void display_message(std::string str) override;

        /// @brief Prints error message to the original terminal buffer.
        void display_error_message(std::string str) override;

        /**
         * @brief Prints board and highlights moves in the terminal.
         * 
         * @param state Board to be displayed, 0 if none.
         * @param moves Moves to be highlighted, 0 if none.
         */
        void display_board(Board &state, uint64_t moves) override;

        /** 
         * @brief Prints game state to the terminal.
         * 
         * @param last_state Last board state.
         * @param current_state Current board state.
         * @param current_color Current player's color.
         * 
         * Displays multiple boards for clarity.
         */
        void display_game(Board &last_state, Board &current_state, bool current_color) override;
};

#endif
