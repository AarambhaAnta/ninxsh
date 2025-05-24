CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include/
SRC = src/main.cpp src/shell.cpp
OUT = ninxsh

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)