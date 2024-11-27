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

#ifndef UI_H
#define UI_H

#include "board/board.h"
#include <string>

class UI {
    public:
        /// @brief Struct containing colors for the UI.
        struct ColorPallete {
            std::string text;
            std::string edge;
            std::string background;
            std::string white;
            std::string black;
            std::string move;
        };

        /// @brief Data structure containing user input
        struct UserInput {
            int x;
            int y;
        };

        /// @brief Collection of pre-defined styles
        enum class UIStyle {
            BASIC,
            SOLARIZED,
            DRACULA
        };

        /// @brief Destroctor of UI. Exits UI.
        virtual ~UI() {};

        /// @return user input
        virtual UserInput get_input() = 0;

        /// @brief Loads specified UI style 
        virtual void load_style(UIStyle style) = 0;

        /// @brief Displays custom message.
        virtual void display_message(std::string str) = 0;

        /// @brief Displays custom error message.
        virtual void display_error_message(std::string str) = 0;

        /**
         * @brief Displays board and highlights moves.
         * 
         * @param state Board to be displayed, 0 if none.
         * @param moves Moves to be highlighted, 0 if none.
         */
        virtual void display_board(Board &state, uint64_t moves) = 0;

        /** 
         * @brief Displays game state.
         * 
         * @param last_state Last board state.
         * @param current_state Current board state.
         * @param current_color Current player's color.
         * 
         * Displays multiple boards for clarity.
         */
        virtual void display_game(Board &last_state, Board &current_state, bool current_color) = 0;

    protected:
        /// @brief Currently loaded color pallete.
        ColorPallete pallete;
};

#endif
