SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp, obj/%.o, $(SRCS))
CFLAGS := -Wall
CC   := g++
EXE  := spider

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $^ -o $@
	./$@

obj/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -rf $(EXE) obj/*

re: clean all
