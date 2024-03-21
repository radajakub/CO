CXX=clang++
# Directory structure
SRC_DIR=src
BUILD_DIR=./build

# Compiler flags
CXXFLAGS=-std=c++11 -O2 -march=native -pthread
INC=-I/Library/gurobi1100/macos_universal2/include
LDFLAGS=
LDLIBS=-L/Library/gurobi1100/macos_universal2/lib -lgurobi_c++ -lgurobi110

# Find all the C++ files in src and test directories
SRCS=$(SRC_DIR)/main.cpp

# Create object file names by replacing .cpp with .o
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Targets
TARGET=$(BUILD_DIR)/solve

# Build targets
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INC) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean test
