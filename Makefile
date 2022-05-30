TARGETS = main
CFLAGS = -g -pedantic -Wall -Wextra -std=c99
all = $(TARGETS)

main: main.o printboard.o checkmove.o minimax.o
	gcc $(CFLAGS) -o main main.o printboard.o checkmove.o minimax.o -lncurses

main.o: main.c board.h checkmove.h minimax.h
	gcc $(CFLAGS) -c main.c

printboard.o: printboard.c board.h
	gcc $(CFLAGS) -c -o $@ $< -lncurses

checkmove.o: checkmove.c board.h checkmove.h
	gcc $(CFLAGS) -c checkmove.c

minimax.o: minimax.c board.h checkmove.h
	gcc $(CFLAGS) -c minimax.c

clean:
	rm -f *.o *~ $(TARGETS) core
