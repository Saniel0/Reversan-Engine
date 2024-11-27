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

#include "ui/ui.h"
#include "engine/engine.h"

class App {
    // public for forward declares
    public:
        /// @brief Collection of app modes.
        enum class Mode {
            PLAY,
            BOT_VS_BOT,
            BENCHMARK
        };

    private:
        /// @brief Currently loaded settings.
        Mode mode;

        /// @brief User interface.
        UI *ui;

        /// @brief Reversi engine.
        Engine *engine;

        /// @brief Runs 'PLAY' mode.
        void run_play();
        
        /// @brief Runs 'BOT_VS_BOT' mode.
        void run_bot_vs_bot();

        /// @brief Runs 'BENCHMARK' mode.
        void run_benchmark();

    public:
        /**
         * @brief Default Terminal constructor.
         * 
         * Loads default settings
         */
        App(Mode mode, UI *ui, Engine *engine);

        /// @brief Run the app with loaded settings
        void run();
};

#endif
