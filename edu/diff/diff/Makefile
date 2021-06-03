SRCS := $(wildcard *.c)
OBJS := $(patsubst %.c, %.o, $(SRCS))
CC   := gcc
EXE  := mydiff

all: $(EXE)

$(EXE): $(OBJS)
	$(CC) $^ -o $@

%.o: %.c
	$(CC) -c $< -o $@


clean:
	rm -rf $(EXE) *.o

re: clean all
