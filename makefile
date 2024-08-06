# Compiler
CXX = clang++

# Source files
SRCS = main.cpp $(wildcard NN/*.cpp) $(wildcard ChessGame/*.cpp)

# Target
TARGET = linchess

# Build rule
$(TARGET): $(SRCS)
	$(CXX) -g -std=c++20 $(SRCS) -o $(TARGET)

# Clean rule
clean:
	rm -f $(TARGET)
