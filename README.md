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
**Reversan Engine** is a powerful Reversi/Othello engine written in C++ that can defeat most above-average players. It employs advanced techniques to ensure efficient, fast searches, including:

- NegaScout game-tree search
- Bitboards
- Transposition tables
- Move reordering

Additionally, **Reversan Engine** leverages AVX2 SIMD instructions, boosting performance by up to 5x (depending on CPU architecture).
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
The engine requires only standard C++ libraries, making the setup straightforward. Follow the instructions below based on your CPU architecture.

#### Standard Installation (for AVX2 Supported CPUs)

If your CPU supports AVX2 instruction set (most modern x86 CPUs), proceed with the default installation:
```bash
git clone https://github.com/Saniel0/Reversan-Engine.git
make
./reversan
```
#### Alternative Installation (for Non-AVX2 CPUs)
If your CPU does not support AVX2, use the `no_simd` flag:
```bash
git clone https://github.com/Saniel0/Reversan-Engine.git
make no_simd
./reversan
```

## Usage
Once compiled, the engine starts in interactive mode, allowing you to play against it. By default, it searches to a depth of 10 moves. You can modify the search depth and other settings by editing the options in `main.cpp`.

## Roadmap
- **Optimizations**: I plan to further optimize the engine, although the core algorithms (find_moves, play_move, rate_board) may have limited optimization potential. Future improvements could include additional state pruning strategies.

- **Multithreading**: Currently, the engine is single-threaded. I aim to implement multithreading in future versions to further improve performance on modern hardware.

## License
Reversan Engine is released under the GNU General Public License (GPLv3). See the [COPYING](./COPYING) file for more information.
