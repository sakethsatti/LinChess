# Compiler
CXX = clang++

# Source files
SRCS = main.cpp board.cpp

# Target
TARGET = linchess

# Build rule
$(TARGET): $(SRCS)
	$(CXX) $(SRCS) -o $(TARGET)

# Clean rule
clean:
	rm -f $(TARGET)