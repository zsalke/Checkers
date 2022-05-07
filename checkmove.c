// checkmove.c
//
// Intended: Checks whether a move is valid.
// Current functionality: returns result of moving a piece in a direction determined by an integer from 0 to 3

#include <stdio.h>
#include "board.h"

struct gamestate {
        int board[8][8];
        int turn;
        int score;
};

struct Space {
	int x;
	int y;
};

struct Space *convertnotation(int sp) {
	return NULL;
}

struct Space *movediagonal(struct Space *pos, int dir) {
	// 0 = +x +y
	// 1 = -x +y
	// 2 = +x -y
	// 3 = -x -y
	/*
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
	*/

	int x = pos->x;
	int y = pos->y;

	if (((y==0) && (dir>=2)) ||
		       ((y==7) && (dir<=1)) ||
	       ((x==0) && ((dir==1) || (dir==3))) ||
		((x==7 && ((dir==0) || (dir==2))))) {
		return NULL;
	}

	struct Space *ptr = (struct Space*) malloc(sizeof(struct Space));
	if (dir==0) {
		x++;
		y++;
	} else if (dir==1) {
		x++;
		y--;
	} else if (dir==2) {
		x--;
		y++;
	} else if (dir==3) {
		x--;
		y--;
	}

	ptr->x = x;
	ptr->y = y;

	return ptr;
}


int printcaptures(struct gamestate *board_struct, struct Space *pos, player, piece_type) {
	printf("TODO: implement captures");
	return 0;
}



int printmoves(struct gamestate *board_struct, struct Space *pos, player) {
// int printmoves(int player) {
	int mych;
	int myking;
	int oppch;
	int oppking;
	int forwardl;
	int forwardr;
	int backl;
	int backr;
	if (player == 2) {
		mych = CHECK;
		myking = KING;
		oppch = XCHECK;
		oppking = XKING;
		forwardl = 3;
		forwardr = 2;
		backl = 1;
		backr = 0;
	} else {
		mych = XCHECK;
		myking = XKING;
		oppch = CHECK;
		oppking = KING;
		forwardl = 0;
		forwardr = 1;
		backl = 2;
		backr = 3;
	}

	struct Space testspace;
	testspace.x = 4;
	testspace.y = 5;
	
	printf("Tests\n");
	printf("Move 26 up left: %d, %d\n", movediagonal(&testspace, 2)->x, movediagonal(&testspace, 2)->y);
	/* printf("Move 6 down left: %d\n", movediagonal(6, 0));
	printf("Try to move backwards from first: %d\n", movediagonal(2, 3));
	printf("Try to move left off board_struct->board: %d\n", movediagonal(13, 0));
	printf("Try to move forwards from last: %d\n", movediagonal(30, 1));
	*/

	int piece = board_struct->board[pos->x][pos->y];
	struct Space *new_space = (struct Space*) malloc(sizeof(struct Space));
	printf("Moves:\n");
	if (piece == mych) {
		new_space = movediagonal(pos, forwardl);
		int new_piece = board_struct->board[new_space->x][new_space->y];
		if (new_piece == 0) {
			printf("%d\n", forwardl);
		} else if ((new_piece==oppch) || (new_piece==oppking)) {
			printcaptures(board_struct, pos, player, piece);
		}

		new_space = movediagonal(pos, forwardr);
		new_piece = board_struct->board[new_space->x][new_space->y];
		if (new_piece == 0) {
			printf("%d\n", forwardr);
		} else if ((new_piece==oppch) || (new_piece==oppking)) {
			printcaptures(board_struct, pos, player, piece);
		}
	} else if (piece == myking) {
		new_space = movediagonal(pos, forwardl);
		int new_piece = board_struct->board[new_space->x][new_space->y];
		if (new_piece == 0) {
			printf("%d\n", forwardl);
		} else if ((new_piece==oppch) || (new_piece==oppking)) {
			printcaptures(board_struct, pos, player, piece);
		}

		new_space = movediagonal(pos, forwardr);
		new_piece = board_struct->board[new_space->x][new_space->y];
		if (new_piece == 0) {
			printf("%d\n", forwardr);
		} else if ((new_piece==oppch) || (new_piece==oppking)) {
			printcaptures(board_struct, pos, player, piece);
		}
		
		new_space = movediagonal(pos, backl);
		new_piece = board_struct->board[new_space->x][new_space->y];
		if (new_piece == 0) {
			printf("%d\n", backl);
		} else if ((new_piece==oppch) || (new_piece==oppking)) {
			printcaptures(board_struct, pos, player, piece);
		}

		new_space = movediagonal(pos, backr);
		new_piece = board_struct->board[new_space->x][new_space->y];
		if (new_piece == 0) {
			printf("%d\n", backr);
		} else if ((new_piece==oppch) || (new_piece==oppking)) {
			printcaptures(board_struct, pos, player, piece);
		}
	}
	
	return 0;
}



int main() {
	struct Space *pos = (struct Space*) malloc(sizeof(struct Space));
	struct gamestate *board_struct = (struct gamestate*) malloc(sizeof(struct gamestate));

	int board[8][8];
	
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++) {
			if (i <= 2) { // AI pieces
				if ((i == 0 || i == 2)) {
					if (j % 2 != 0) {
						board_struct->board[i][j] = XCHECK; //global var
					} else {
						board_struct->board[i][j] = EMPTY; 
					}
				} else {
					if (j % 2 == 0) {
						board_struct->board[i][j] = XCHECK; //global var
					} else {
						board_struct->board[i][j] = EMPTY; 
					}
				}
			}
			if (i >= 5) { // player pieces
				if ((i == 5 || i == 7)) {
					if (j % 2 == 0) {
						board_struct->board[i][j] = CHECK; //global var
					} else {
						board_struct->board[i][j] = EMPTY; 
					}
				} else {
					if (j % 2 != 0) {
						board_struct->board[i][j] = CHECK; //global var
					} else {
						board_struct->board[i][j] = EMPTY; 
					}
				}
			}
		}
	}



	pos->x = 3;
	pos->y = 2;
	printmoves(board_struct, pos, 2);
	return 0;
}
