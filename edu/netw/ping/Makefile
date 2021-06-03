SRCS := $(wildcard src/*.c)
OBJS := $(patsubst src/%.c, obj/%.o, $(SRCS))
CC   := gcc
EXE  := cockroach

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $^ -o $@

obj/%.o: src/%.c
	$(CC) -c $< -o $@


clean:
	rm -rf $(EXE) obj/*

re: clean all

