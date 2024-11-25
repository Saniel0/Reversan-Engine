#	This file is part of Reversan Engine.
#
#	Reversan Engine is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   Reversan Engine is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with Reversan Engine. If not, see <https://www.gnu.org/licenses/>.

# Minimum CMake version required
cmake_minimum_required(VERSION 3.12)

# Project name and supported languages
project(ReversanEngine LANGUAGES CXX)

# Compiler settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -Wno-attributes")

# Set the output directory for binaries
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Source files common to all builds
set(SOURCES
    src/main.cpp
    src/app.cpp
    src/board_state.cpp
    src/terminal.cpp
    src/transposition_table.cpp
    src/move_order.cpp
    src/negascout.cpp
    src/alphabeta.cpp
    src/parser.cpp
)

# Source files for the no SIMD variant
set(SOURCES_NOSIMD
    src/board_nosimd.cpp
)

# Source files for the AVX2 variant
set(SOURCES_AVX2
    src/board_avx2.cpp
)

# Include directories
include_directories(include)

# Check for AVX2 support
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-mavx2" AVX2)

# Create AVX2 targets
if(AVX2)
    add_executable(reversan_avx2 ${SOURCES} ${SOURCES_AVX2})
    target_compile_options(reversan_avx2 PRIVATE -mavx2 -flto)
    target_link_libraries(reversan_avx2 PRIVATE -flto)

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_executable(reversan_avx2_debug ${SOURCES} ${SOURCES_AVX2})
        target_compile_options(reversan_avx2_debug PRIVATE -mavx2 -pg)
        target_link_libraries(reversan_avx2_debug PRIVATE -pg)
    endif()
endif()

# Create the No SIMD build target
add_executable(reversan_nosimd ${SOURCES} ${SOURCES_NOSIMD})
target_compile_options(reversan_nosimd PRIVATE -flto)
target_link_libraries(reversan_nosimd PRIVATE -flto)

# Debug version for No SIMD
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(reversan_nosimd_debug ${SOURCES} ${SOURCES_NOSIMD})
    target_compile_options(reversan_nosimd_debug PRIVATE -pg)
    target_link_libraries(reversan_nosimd_debug PRIVATE -pg)
endif()

# Custom clean command to remove the build directory (optional, CMake does clean by itself)
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/reversan_avx2
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/reversan_avx2_debug
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/reversan_nosimd
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/reversan_nosimd_debug
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_SOURCE_DIR}/build
)
