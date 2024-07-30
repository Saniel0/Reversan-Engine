#ifndef TERMINAL_H
#define TERMINAL_H

#include "board.h"

void print_title_blur();
void print_title_3d();
void print_title_tile();
void delete_lines(int count);
void print_boards(Board *last_state, Board *current_state, bool current_color);

#endif
