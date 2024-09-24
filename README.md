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
░▒▓███████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░▒▓███████▓▒░ ░▒▓███████▓▒░░▒▓██████▓▒░░▒▓███████▓▒░
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░       ░▒▓█▓▒▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
░▒▓███████▓▒░░▒▓██████▓▒░  ░▒▓█▓▒▒▓█▓▒░░▒▓██████▓▒░ ░▒▓███████▓▒░ ░▒▓██████▓▒░░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░        ░▒▓█▓▓█▓▒░ ░▒▓█▓▒░      ░▒▓█▓▒░░▒▓█▓▒░      ░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
░▒▓█▓▒░░▒▓█▓▒░▒▓████████▓▒░  ░▒▓██▓▒░  ░▒▓████████▓▒░▒▓█▓▒░░▒▓█▓▒░▒▓███████▓▒░░▒▓█▓▒░░▒▓█▓▒░▒▓█▓▒░░▒▓█▓▒░
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
**Reversan Engine** requires only standard C++ libraries, making the setup straightforward. Follow the instructions below based on your preferred installation method and CPU architecture.

### CMake (recommended)
Using `cmake` is the default and recommended method for installing **Reversan Engine**. This method automatically detects whether your CPU supports AVX2.
1. **Build the Project:**
   ```bash
   git clone https://github.com/Saniel0/Reversan-Engine.git
   cmake -B build
   cmake --build build
   ```
    This will build both AVX2 (if supported) and non-AVX2 versions.
1. **Debug Build (Optional):** To create additional debug builds for profiling with `gprof`, use
    ```bash
    git clone https://github.com/Saniel0/Reversan-Engine.git
    cmake -B build -DCMAKE_BUILD_TYPE=Debug
    cmake --build build
    ```
### Make
You can also install **Reversan Engine** using `make`. This method provides a simpler setup but requires manual selection of the AVX2 support.
1. **AVX2 Version:**
    ```bash
    git clone https://github.com/Saniel0/Reversan-Engine.git
    make
    ```
2. **Non-AVX2 version:**
    ```bash
    git clone https://github.com/Saniel0/Reversan-Engine.git
    make no_simd
    ```

## Usage
#### Play against the engine
```bash
reversan --play
```
#### Let the engine play against itself
```bash
reversan --bot-vs-bot
```
#### Benchmark engine efficiency on pre-defined state
```bash
reversan --benchmark
```
#### For additional options and details, run
```bash
reversan --help
```

## Roadmap
- **Optimizations**: I plan to further optimize the engine, although the core algorithms (find_moves, play_move, rate_board) may have limited optimization potential. Future improvements could include additional state pruning strategies.

- **Multithreading**: Currently, the engine is single-threaded. I aim to implement multithreading in future versions to further improve performance on modern hardware.

## License
Reversan Engine is released under the GNU General Public License (GPLv3). See the [COPYING](./COPYING) file for more information.
