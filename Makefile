TARGETS = main
CFLAGS = -g -pedantic -Wall -Wextra -std=c99
all = $(TARGETS)

main: main.c printboard.o checkmove.o
	gcc $(CFLAGS) -o main main.c printboard.o checkmove.o -lncurses

printboard.o: printboard.c board.h
	gcc $(CFLAGS) -c -o $@ $< -lncurses

checkmove.o: checkmove.c board.h checkmove.h
	gcc $(CFLAGS) -c checkmove.c

clean:
	rm -f *.o *~ $(TARGETS) core
