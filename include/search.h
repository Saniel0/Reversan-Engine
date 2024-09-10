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

#ifndef SEARCH_H
#define SEARCH_H

#include <cstdint>
#include "board.h"
#include "transposition_table.h"

class Search {
    private:
        // used only for statistics
        unsigned long long int heuristic_count;
        unsigned long long int state_count;
        
        // order in which possible moves are checked, important for performance reasons
        uint64_t move_order[64];

        Transposition_table transposition_table;
        
        int negascout(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board);
        int minimax(Board *state, int depth, bool cur_color, int alpha, int beta, bool end_board);
        //int heuristics(Board *state, int moves_delta, bool end_board);

    public:
        Search();
        uint64_t start_negascout(Board *state, bool color, int search_depth);
        uint64_t start_minimax(Board *state, bool color, int search_depth);
};

#endif
