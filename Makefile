.POSIX:

PREFIX = /usr/local

CC = c89 -Wall
AR = ar

SRC = memdebug.c hand_err.c stack.c
OBJ = $(SRC:.c=.o)

all: dynamic

.c.o:
	$(CC) -fPIC -c $<

memdebug.o: memdebug.h stack.h types.h hand_err.h
hand_err.o: stack.h types.h hand_err.h
stack.o: stack.h types.h hand_err.h

static: $(OBJ)
	$(AR) rcs libmem-debug.a $(OBJ)

dynamic: $(OBJ)
	$(CC) -shared -o libmem-debug.so $(OBJ)

clean:
	rm -f libmem-debug.so libmemdebug.a $(OBJ)

install: dynamic
	cp -f libmem-debug.so $(PREFIX)/lib
	cp -f libmem-debug.h $(PREFIX)/include

uninstall:
	rm -f $(PREFIX)/lib/libmem-debug.so
	rm -f $(PREFIX)/include/libmem-debug.h

.PHONY: dynamic static clean install uninstall




