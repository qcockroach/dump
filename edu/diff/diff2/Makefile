SRCS   := $(wildcard *.c)
OBJS   := $(patsubst %.c, %.o, $(SRCS))
CC     := gcc
CFLAGS := -I inc -Wall
LFLAGS := 
PRJ    := roach_diff

all: $(PRJ)


$(PRJ): $(OBJS)
	$(CC) $^ -o $@ $(LFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
ifeq ($(PRJ), $(wildcard $(PRJ)))
	rm -rf $(PRJ) *.o
endif
	clear

re: clean all


run:
	@./$(PRJ) data/file1 data/file2 --normal
