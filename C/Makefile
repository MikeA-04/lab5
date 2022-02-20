SRC = $(wildcard *.c)
BINARIES = $(patsubst %.c, %, $(SRC))

all: $(BINARIES)

$(BINARIES): % : %.c
	gcc -o $@ $^ -lncurses

clean:
	rm $(BINARIES)
