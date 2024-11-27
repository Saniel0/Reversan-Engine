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
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Include directories
include_directories(include)

# Source files common to all builds
set(SOURCES
    src/main.cpp
    src/app/app.cpp
    src/board/board_state.cpp
    src/engine/alphabeta.cpp
    src/engine/move_order.cpp
    src/engine/negascout.cpp
    src/engine/transposition_table.cpp
    src/ui/terminal.cpp
    src/utils/parser.cpp
    src/utils/thread_manager.cpp
)

# Source files for the no SIMD variant
set(SOURCES_NOSIMD
    src/board/board_nosimd.cpp
)

# Source files for the AVX2 variant
set(SOURCES_AVX2
    src/board/board_avx2.cpp
)

# Compile flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-attributes")
    set(OPT_FLAG "-O3")
    set(PROFILE_FLAG "-pg")
    set(AVX2_FLAG "-mavx2")
    message(STATUS "GNU | Clang compiler detected")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
    set(OPT_FLAG "/O2")
    set(PROFILE_FLAG "/PROFILE")
    set(AVX2_FLAG "/arch:AVX2")
    message(STATUS "MSVC compiler detected")
else()
    message(FATAL_ERROR "Unsupported compiler")
endif()


# Check AVX2 support and inform user
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag(${AVX2_FLAG} AVX2)
if(AVX2)
    message(STATUS "AVX2 support detected")
else()
    message(STATUS "AVX2 not supported")
endif()

# Check LTO/IPO support and inform user
include(CheckIPOSupported)
check_ipo_supported(RESULT LTO)
if(LTO)
    message(STATUS "LTO support detected")
else()
    message(STATUS "LTO not supported")
endif()

# Create AVX2 targets
if(AVX2)
    add_executable(reversan_avx2 ${SOURCES} ${SOURCES_AVX2})
    target_compile_options(reversan_avx2 PRIVATE ${OPT_FLAG} ${AVX2_FLAG})
    if(LTO)
        set_target_properties(reversan_avx2 PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
    endif()

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_executable(reversan_avx2_debug ${SOURCES} ${SOURCES_AVX2})
        target_compile_options(reversan_avx2_debug PRIVATE ${PROFILE_FLAG} ${AVX2_FLAG})
        target_link_libraries(reversan_avx2_debug PRIVATE ${PROFILE_FLAG})
    endif()
endif()

# Create the No SIMD build target
add_executable(reversan_nosimd ${SOURCES} ${SOURCES_NOSIMD})
target_compile_options(reversan_nosimd PRIVATE ${OPT_FLAG})
if(LTO)
    set_target_properties(reversan_nosimd PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Debug version for No SIMD
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(reversan_nosimd_debug ${SOURCES} ${SOURCES_NOSIMD})
    target_compile_options(reversan_nosimd_debug PRIVATE ${PROFILE_FLAG})
    target_link_libraries(reversan_nosimd_debug PRIVATE ${PROFILE_FLAG})
endif()

# Custom clean command to remove the build directory (optional, CMake does clean by itself)
add_custom_target(clean_all
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/reversan_avx2
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/reversan_avx2_debug
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/reversan_nosimd
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_SOURCE_DIR}/reversan_nosimd_debug
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
)
