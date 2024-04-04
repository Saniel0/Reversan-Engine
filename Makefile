CXX = g++
LINKER = g++

CXX_FLAGS = -O3 -Wall
LINKER_FLAGS =

SOURCES = main.cpp board.cpp reversi.cpp
OBJECTS += $(filter %.o,$(SOURCES:%.cpp=%.o))
TARGET_EXE = reversi

all: CXX_FLAGS += -mavx2
all: $(TARGET_EXE)

no_simd: $(TARGET_EXE)

clean:
	rm -f $(OBJECTS) $(TARGET_EXE)

$(TARGET_EXE) : $(OBJECTS)
	$(LINKER) $(LINKER_FLAGS) $^ -o $@

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@