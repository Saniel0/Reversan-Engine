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

#include "ui/ui.h"

class Terminal : public UI {
    private:
        /// @brief Collection of ANSI escape sequences.
        struct Escapes {
            // buffer control
            static constexpr const char* SWITCH_BUFFER  = "\x1B[?1049h";
            static constexpr const char* RESTORE_BUFFER = "\x1B[?1049l";
            static constexpr const char* CLEAR_BUFFER   = "\x1B[2J\x1B[H";
            // special graphics
            static constexpr const char* BLINK          = "\e[5m";
            static constexpr const char* BLINK_RESET    = "\e[25m";
            static constexpr const char* BOLD           = "\e[1m";
            // basic color control
            static constexpr const char* RED            = "\e[1;31m";
            static constexpr const char* YELLOW         = "\e[1;33m";
            static constexpr const char* GREEN          = "\e[1;32m";
            static constexpr const char* GRAY           = "\e[1;90m";
            static constexpr const char* WHITE          = "\e[1;37m";
            // basic background control
            static constexpr const char* BACK_NONE      = "";
            static constexpr const char* BACK_BLACK     = "\e[40m";
            static constexpr const char* BACK_RED       = "\e[41m";
            static constexpr const char* BACK_GREEN     = "\e[42m";
            static constexpr const char* BACK_YELLOW    = "\e[43m";
            static constexpr const char* BACK_BLUE      = "\e[44m";
            static constexpr const char* BACK_PURPLE    = "\e[45m";
            static constexpr const char* BACK_CYAN      = "\e[46m";
            static constexpr const char* BACK_WHITE     = "\e[47m";
        };
        
        /**
         * @brief Generates UI block in form of a string.
         * 
         * @param left_size Size of the left blank edge.
         * @param block_size Size of the middle block with pattern.
         * @param right_size Size of the right blank edge.
         * @param style Style of the pattern.
         * @param parity Parity of the pattern on the middle block.
         */
        std::string row_block(int left_size, int block_size, int right_side, bool parity) const;

        /// @brief Clears whole terminal
        void clear_terminal() const;

        /// @brief Prints title to the terminal
        void print_title() const;

    public:
        /// @brief Switches to separate terminal buffer.
        explicit Terminal(UIStyle style = UIStyle::BASIC);

        /// @brief Alternate constructor for loading custom color palletes. 
        explicit Terminal(ColorPallete pallete);
        
        /// @brief Restores original terminal buffer.
        ~Terminal() override;

        /// @return User input from terminal.
        UserInput get_input() override;

        /// @brief Loads wanted style. 
        void load_style(UIStyle style) override;

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
