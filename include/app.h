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

#ifndef APP_H
#define APP_H

#include "ui.h"

class App {
    private:
        /// @brief Struct containing app settings.
        struct Settings {
            int mode;
            int search_depth;
            int search_method;
        };

        /// @brief Currently loaded settings.
        Settings settings;

        /// @brief User interface
        UI *ui;

        void run_play();
        
        void run_bot_vs_bot();

        void run_benchmark();

    public:
        /// @brief Collection of app modes.
        enum Modes {
            PLAY,
            BOT_VS_BOT,
            BENCHMARK
        };

        /// @brief Collection of engine search algorithms.
        enum Engine {
            MINIMAX,
            NEGASCOUT
        };

        /**
         * @brief Default Terminal constructor.
         * 
         * Loads default settings
         */
        App(UI *ui);

        /**
         * @brief Terminal constructor with custom settings.
         * 
         * @param mode App mode
         * @param search_depth Engine search depth.
         * @param search_method Engine search method.
         */
        App(UI *ui, int mode, int search_depth, int search_method);

        /**
         * @brief Loads settings based on arguments.
         * 
         * @param argc Argument count.
         * @param argv Argument values.
         * 
         * @return true if settings were parsed correctly, false otherwise
         */
        bool parse_settings(int argc, char **argv);

        /**
         * @brief Run the app with loaded settings
         */
        void run();
};

#endif
