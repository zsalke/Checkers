TARGETS = printboardnc
CFLAGS = -g -pedantic -Wall -Wextra -std=c99
all = $(TARGETS)

printboardnc: printboard.c board.h
	gcc $(CFLAGS) -o $@ $< -lncurses
