<!--
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
-->

# Reversan Engine
Reversan is powerfull and fast reversi engine with terminal interface. Only uses standard c++ libraries.

The goal was to learn several concepts to create very optimized engine. The engine uses bitboards alongside AVX2 SIMD instructions, transposition tables, negascout with optimized move ordering and much more to achieve the best performance.
```c++
░▒▓███████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓███████▓▒░ ░▒▓███████▓▒░▒▓█▓▒░
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░       ░▒▓█▓▒▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░
░▒▓███████▓▒░░▒▓██████▓▒░  ░▒▓█▓▒▒▓█▓▒░░▒▓██████▓▒░ ░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓█▓▒░
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░
░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░  ░▒▓██▓▒░  ░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░░▒▓█▓▒░
  0 1 2 3 4 5 6 7        0 1 2 3 4 5 6 7     |       0 1 2 3 4 5 6 7
0 - X X X + + + -      0 - X X X - - - -     |     0 - X X X - - - - 
1 - + X X X X - X      1 - - X X X X - X     |     1 - + X X X X - X     WHITE
2 O O X O O O X X      2 O O X O O O X X     |     2 O O X O O O X X     17
3 + X O O O O X X      3 - X O O O O X X     |     3 + X O O O O X X 
4 X + O X X + X +      4 X - O O O O X -     |     4 X + O O O O X - 
5 - O + + + + X +      5 - O - - - - X -     |     5 - O + + + + X -     BLACK
6 - - - - - - - -      6 - - - - - - - -     |     6 + - + - - - - -     14
7 - - - - - - - -      7 - - - - - - - -     |     7 - - - - - - - - 
Select move > 
```

## Installation
Since the project only uses standard c++ libraries, the setup is quite straight forward.
- The engine makes heavy use of AVX2 instructions to achieve best performance, if your CPU supports these instruction, go forward with defult installation
```bash
git clone https://github.com/Saniel0/Reversan-Engine.git
make
./reversi
```
- If your CPU does not support AVX2 instructions, set no_simd make flag
```bash
git clone https://github.com/Saniel0/Reversan-Engine.git
make no_simd
./reversi
```

## Usage
After you compile the project, it starts in interactive mode, where you play against the engine. The engine has constant search depth 10 by default. If you want to change this, there are several options in main.cpp

## Roadmap
I would like to optimize the engine further, I believe there is not that much room to optimize logic of underlying algorithms (such as find_moves, play_move, rate_board), but I believe there are still ways to prune out more states. I would also like to utilize multithreading in the future, since the code is currently completely single-threaded.
