CXX = g++
LINKER = g++

CXX_FLAGS = -O3 -Wall
LINKER_FLAGS =

# Sources for every build
SOURCES = main.cpp board_print.cpp board_state.cpp reversi.cpp terminal.cpp transposition_table.cpp
OBJECTS = $(filter %.o,$(SOURCES:%.cpp=%.o))

# Sources when building without any explicit SIMD instructions
SOURCES_NOSIMD = board.cpp
OBJECTS_NOSIMD = $(OBJECTS) $(filter %.o,$(SOURCES_NOSIMD:%.cpp=%.o))

# Sources when building with explicit AVX2 instructions
SOURCES_AVX2 = board_avx2.cpp
OBJECTS_AVX2 = $(OBJECTS) $(filter %.o,$(SOURCES_AVX2:%.cpp=%.o))

TARGET_EXE = reversi

all: CXX_FLAGS += -mavx2
all: $(OBJECTS_AVX2)
	$(LINKER) $(LINKER_FLAGS) $^ -o $(TARGET_EXE)

debug: CXX_FLAGS += -pg
debug: LINKER_FLAGS += -pg
debug: all

no_simd: MACROS += -D NO_SIMD
no_simd: $(OBJECTS_NOSIMD)
	$(LINKER) $(LINKER_FLAGS) $^ -o $(TARGET_EXE)

debug_no_simd: CXX_FLAGS += -pg
debug_no_simd: LINKER_FLAGS += -pg
debug_no_simd: no_simd

clean:
	rm -f $(OBJECTS_AVX2) $(OBJECTS_NOSIMD) $(TARGET_EXE)

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@ $(MACROS)
