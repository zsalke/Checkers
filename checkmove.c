// checkmove.c
//
// Intended: Checks whether a move is valid.
// Current functionality: returns result of moving a piece in a direction determined by an integer from 0 to 3

#include <stdio.h>

#define BOARD_SIZE 16
#define EMPTY 0
#define CHECK 1
#define KING 2
#define XCHECK 3
#define XKING 4

int movediagonal(int space, int dir) {
	// 0 = smaller increment move
	// 1 = larger increment move
	// 2 = smaller decrement move
	// 3 = larger decrement move
	int mv;

	if (space<4 && dir>=2) { // cannot move backwards from first row
		return -1;
	} else if (space >=29 && dir<=1) { // cannot move forward from last row
		return -1;
	} else if (dir == 0) {
		mv = 4;
	} else if (dir == 1) {
		mv = 5;
	} else if (dir == 2) {
		mv = -3;
	} else { // if dir == 3
		mv = -4;
	}
		
	if (space%8 > 4 || space%8 == 0) {
		mv--;
	}
	printf("Move size: %d\n", mv);

	// prevent moving across board
	int dest = space + mv;

	if ((space%8==5) && (dest%8==0)) {
		return -1;
	} else if (dir == 0 || dir == 2) {
		if (dest==12 || dest==20 || dest==28) {
			return -1;
		}
	} else if (dir == 1 || dir == 3) {
		if (dest==5 || dest==13 || dest==21 || dest==29) {
			return -1;
		}
	}

	return dest;
}

// int canmove(int gamestate[], int piece, player) {
int canmove(int player) {
	int mych;
	int myking;
	if (player == 2) {
		mych = CHECK;
		myking = KING;
	} else {
		mych = XCHECK;
		myking = XKING;
	}

	printf("Move 26 up right: %d\n", movediagonal(26, 2));
	printf("Move 6 down left: %d\n", movediagonal(6, 0));
	printf("Try to move backwards from first: %d\n", movediagonal(2, 3));
	printf("Try to move left off board: %d\n", movediagonal(13, 0));
	printf("Try to move forwards from last: %d\n", movediagonal(30, 1));

	//if (gamestate[piece] == mych) {
	//	if (
	
	return 0;
}

int main() {
	canmove(1);
	return 0;
}
