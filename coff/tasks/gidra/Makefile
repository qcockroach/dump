SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SRCS))
CC   := g++
PRJ := gidra

all: $(PRJ)

$(PRJ): $(OBJS)
	$(CC) $^ -o $@
	./$@

%.o: %.cpp
	$(CC) -c $< -o $@

clean:
	rm -f *.o

re: clean all

