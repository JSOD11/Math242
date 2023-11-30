# Compiler settings - Can be customized.
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Target executable name
TARGET = simulation

# Source files
SOURCES = simulation.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Rule to create object files from source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Default rule
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# Clean rule
clean:
	rm -f $(OBJECTS) $(TARGET)

# Rule for debugging
debug: CXXFLAGS += -g
debug: $(TARGET)
