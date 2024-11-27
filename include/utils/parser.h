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

#ifndef PARSER_H
#define PARSER_H

#include "app/app.h"
#include "app/default_settings.h"

class Parser {
    private:
        App::Mode mode;
        UI::UIStyle style;
        Engine::Alg alg;
        Engine::Settings settings;

        /// @brief Prints help message to terminal.
        void print_help() const;

        /// @brief Tries to parse mode. 
        bool parse_mode(int argc, char **argv);

        /// @brief Tries to parse depth.
        bool parse_depth(int argc, char **argv, int &i);

        /// @brief Tries to parse engine.
        bool parse_engine(int argc, char **argv, int &i);

        /// @brief Tries to parse style.
        bool parse_style(int argc, char **argv, int &i);

        /// @brief Tries to parse thread count.
        bool parse_threads(int argc, char **argv, int &i);

    public:
        Parser();

        /// @brief Parses arguments from the command line.
        bool parse(int argc, char **argv);

        App::Mode get_mode() const;
        UI::UIStyle get_style() const;
        Engine::Alg get_alg() const;
        Engine::Settings get_settings() const;
};

#endif
