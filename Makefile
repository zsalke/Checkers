TARGETS = printboardnc
CFLAGS = -g -pedantic -Wall -Wextra -std=c99
all = $(TARGETS)

main: main.c printboard.o
	gcc $(CFLAGS) -o main main.c printboard.o -lncurses
printboard.o: printboard.c board.h
	gcc $(CFLAGS) -c -o $@ $< -lncurses
