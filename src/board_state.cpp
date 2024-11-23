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

// __atribute__ is not standard, enable only with supported compillers
#if defined(__GNUC__) || defined(__clang__)
#define ALWAYS_INLINE __attribute__((always_inline))
#else
#define ALWAYS_INLINE
#endif

#include "board.h"
#include <bit>

Board::Board() : white_bitmap(0), black_bitmap(0) {}

Board::Board(const uint64_t white_bitmap, const uint64_t black_bitmap) : white_bitmap(white_bitmap), black_bitmap(black_bitmap) {}

ALWAYS_INLINE int Board::count_white() const {
    return std::popcount(white_bitmap);
}

ALWAYS_INLINE int Board::count_black() const {
    return std::popcount(black_bitmap);
}

ALWAYS_INLINE uint64_t Board::hash() const {
    // Murmur hash of white
    uint64_t w = white_bitmap;
    w ^= w >> 33;
    w *= 0xff51afd7ed558ccd;
    w ^= w >> 33;
    w *= 0xc4ceb9fe1a85ec53;
    w ^= w >> 33;
    // Murmur hash of black
    uint64_t b = black_bitmap;
    b ^= b >> 33;
    b *= 0xff51afd7ed558ccd;
    b ^= b >> 33;
    b *= 0xc4ceb9fe1a85ec53;
    b ^= b >> 33;
    // Combine white and black hashes
    return w ^ b;
}

const Board Board::States::initial = Board(
    // or operations just for readability
    // static cast so the number is not simple integer - shifting would go out of range
    // WHITE
    static_cast<uint64_t>(0b00000000) << 56 |
    static_cast<uint64_t>(0b00000000) << 48 |
    static_cast<uint64_t>(0b00000000) << 40 |
    static_cast<uint64_t>(0b00010000) << 32 |
    static_cast<uint64_t>(0b00001000) << 24 |
    static_cast<uint64_t>(0b00000000) << 16 |
    static_cast<uint64_t>(0b00000000) << 8 |
    static_cast<uint64_t>(0b00000000),
    // BLACK
    static_cast<uint64_t>(0b00000000) << 56 |
    static_cast<uint64_t>(0b00000000) << 48 |
    static_cast<uint64_t>(0b00000000) << 40 |
    static_cast<uint64_t>(0b00001000) << 32 |
    static_cast<uint64_t>(0b00010000) << 24 |
    static_cast<uint64_t>(0b00000000) << 16 |
    static_cast<uint64_t>(0b00000000) << 8 |
    static_cast<uint64_t>(0b00000000)
);

const Board Board::States::test = Board(
    // random board used to validate functions
    // or operations just for readability
    // static cast so the number is not simple integer - shifting would go out of range
    // WHITE
    static_cast<uint64_t>(0b01000000) << 56 |
    static_cast<uint64_t>(0b01000000) << 48 |
    static_cast<uint64_t>(0b00010000) << 40 |
    static_cast<uint64_t>(0b00001000) << 32 |
    static_cast<uint64_t>(0b00010100) << 24 |
    static_cast<uint64_t>(0b00000000) << 16 |
    static_cast<uint64_t>(0b00000000) << 8 |
    static_cast<uint64_t>(0b00000000),
    // BLACK
    static_cast<uint64_t>(0b00000000) << 56 |
    static_cast<uint64_t>(0b00000000) << 48 |
    static_cast<uint64_t>(0b11100000) << 40 |
    static_cast<uint64_t>(0b00010000) << 32 |
    static_cast<uint64_t>(0b00001000) << 24 |
    static_cast<uint64_t>(0b00000100) << 16 |
    static_cast<uint64_t>(0b00000000) << 8 |
    static_cast<uint64_t>(0b00000000)
);

const Board Board::States::benchmark = Board(
    // one of the more difficult board states to compute
    // or operations just for readability
    // static cast so the number is not simple integer - shifting would go out of range
    // WHITE
    static_cast<uint64_t>(0b00000001) << 56 |
    static_cast<uint64_t>(0b00010010) << 48 |
    static_cast<uint64_t>(0b01111110) << 40 |
    static_cast<uint64_t>(0b01111100) << 32 |
    static_cast<uint64_t>(0b00111100) << 24 |
    static_cast<uint64_t>(0b00010100) << 16 |
    static_cast<uint64_t>(0b00110000) << 8 |
    static_cast<uint64_t>(0b00010000),
    // BLACK
    static_cast<uint64_t>(0b11111100) << 56 |
    static_cast<uint64_t>(0b00101100) << 48 |
    static_cast<uint64_t>(0b00000000) << 40 |
    static_cast<uint64_t>(0b00000000) << 32 |
    static_cast<uint64_t>(0b11000000) << 24 |
    static_cast<uint64_t>(0b11101000) << 16 |
    static_cast<uint64_t>(0b00000000) << 8 |
    static_cast<uint64_t>(0b00000000)
);
