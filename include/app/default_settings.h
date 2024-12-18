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

#ifndef DEFAULT_SETTINGS_H
#define DEFAULT_SETTINGS_H

#include "app/app.h"

/// @brief Default settings for the whole project.
struct DefaultSettings {
    static constexpr App::Mode MODE = App::Mode::PLAY;
    static constexpr UI::UIStyle STYLE = UI::UIStyle::BASIC;
    static constexpr Engine::Alg ALG = Engine::Alg::NEGASCOUT;
    static constexpr Engine::Settings SETTINGS = {10, 0, 1, true, Move_order::Orders::OPTIMIZED};
};

#endif
