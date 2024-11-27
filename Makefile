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

# Select compiller of your choice, tested only with g++
CXX = g++
LINKER = g++

# Add include path
CXX_FLAGS = -Iinclude

# Select compile flags
CXX_FLAGS += -std=c++20 -O3 -flto -Wall -Wno-attributes
LINKER_FLAGS = -flto

# Add source and build path
SOURCE_DIR = src
BUILD_DIR = build

# Sources for every build
SOURCES  = main.cpp
SOURCES += app/app.cpp
SOURCES += board/board_state.cpp
SOURCES += engine/alphabeta.cpp
SOURCES += engine/move_order.cpp
SOURCES += engine/negascout.cpp
SOURCES += engine/transposition_table.cpp
SOURCES += ui/terminal.cpp
SOURCES += utils/parser.cpp
SOURCES += utils/thread_manager.cpp
OBJECTS = $(addprefix $(BUILD_DIR)/,$(SOURCES:%.cpp=%.o))

# Sources when building without any explicit SIMD instructions
SOURCES_NOSIMD = board/board_nosimd.cpp
OBJECTS_NOSIMD = $(OBJECTS) $(addprefix $(BUILD_DIR)/,$(SOURCES_NOSIMD:%.cpp=%.o))

# Sources when building with explicit AVX2 instructions
SOURCES_AVX2 = board/board_avx2.cpp
OBJECTS_AVX2 = $(OBJECTS) $(addprefix $(BUILD_DIR)/,$(SOURCES_AVX2:%.cpp=%.o))

# Name of final executable
TARGET_EXE = reversan

all: CXX_FLAGS += -mavx2
all: $(OBJECTS_AVX2)
	$(LINKER) $(LINKER_FLAGS) $^ -o $(TARGET_EXE)

debug: CXX_FLAGS += -pg
debug: LINKER_FLAGS += -pg
debug: all

no_simd: $(OBJECTS_NOSIMD)
	$(LINKER) $(LINKER_FLAGS) $^ -o $(TARGET_EXE)

debug_no_simd: CXX_FLAGS += -pg
debug_no_simd: LINKER_FLAGS += -pg
debug_no_simd: no_simd

clean:
	rm -f -r $(BUILD_DIR)
	rm -f $(TARGET_EXE)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXX_FLAGS) -c $< -o $@

# Create build directory if it does not exist
$(BUILD_DIR):
	mkdir $@
