# Compiler
CXX = clang++

# Source files
SRCS = main.cpp $(wildcard NN/*.cpp) $(wildcard ChessGame/*.cpp)

# Target
TARGET = linchess

# Build rule
$(TARGET): $(SRCS)
	$(CXX) $(SRCS) -o $(TARGET) -std=c++20

# Clean rule
clean:
	rm -f $(TARGET)