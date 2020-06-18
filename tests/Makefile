CC = c89

CFLAGS = -Wall -DLIBMEM_DEBUG_DISABLE
LDFLAGS = -L.. -lmem-debug

simple-print_SRC = simple-print.c
simple-print_OBJ = ${simple-print_SRC:.c=.o}

all: simple-print

.c.o:
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f simple-print *.o

simple-print: $(simple-print_OBJ)
	$(CC) -o $@ $(simple-print_OBJ) $(LDFLAGS)
	@echo
	@echo "\033[0;36mRunning simple-print\033[0;3m"
	LD_LIBRARY_PATH=.. ./simple-print
	@echo "\033[0m"

.PHONY: all clean simple-print
