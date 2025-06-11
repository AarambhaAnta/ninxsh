CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include/
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
BIN = ninxsh

all: $(BIN)

$(BIN): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(BIN) src/*.o
