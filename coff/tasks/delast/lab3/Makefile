SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp, objs/%.o, $(SRCS))
CFLAGS := -Wall
CC   := g++
EXE  := lab3


all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $^ -o $@

objs/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(EXE) objs/*

re: clean all
