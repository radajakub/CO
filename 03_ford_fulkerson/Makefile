CXX=clang++
# Directory structure
SRC_DIR=src
BUILD_DIR=./build

# Compiler flags
CXXFLAGS=-std=c++11 -O2 -march=native -pthread

# Find all the C++ files in src and test directories
SRCS=$(SRC_DIR)/network.cpp $(SRC_DIR)/ford_fulkerson.cpp $(SRC_DIR)/main.cpp

# Create object file names by replacing .cpp with .o
OBJS=$(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Targets
TARGET=$(BUILD_DIR)/solve

# Build targets
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR)

.PHONY: clean all
