// checkmove.c
//

#include <stdio.h>
#include <string.h>
#include "board.h"

struct gamestate {
        int board[8][8];
        int turn;
        int score;
	int past_x;
	int past_y;
	int curr_x;
	int curr_y;
};

struct LinkedList {
	struct gamestate *move;
	struct LinkedList *next;
};

void append(struct LinkedList *list, struct gamestate *new) {
	while (list->next) {
		list = list->next;
	}
	list->next = malloc(sizeof(struct LinkedList));
	list->next->move = new;
	list->next->next = NULL;
}

void printcaptures(struct gamestate *board_struct, int x, int y, int ydir, bool king, struct LinkedList *move_list) {
	// TODO implement kings	
	printf("Print captures\n");
	if ((x>1 && (y+2*ydir>=0 && y+2*ydir<=8)) && ((board_struct->board[y+ydir][x-1] == XCHECK) || (board_struct->board[y+ydir][x-1] == XKING))) {
		if (board_struct->board[y+2*ydir][x-2] == 0) {
			printf("%d, %d\n", x-2, y+2*ydir);
			struct gamestate *new_move = malloc(sizeof(struct gamestate));
			memcpy(new_move, board_struct, sizeof(struct gamestate));
			new_move->board[y+2*ydir][x-2] = new_move->board[y][x];
			new_move->board[y+ydir][x-1] = 0;
			new_move->board[y][x] = 0;
			new_move->curr_x = x-2;
			new_move->curr_y = y+2*ydir;

			append(move_list, new_move);
			
			printcaptures(board_struct, x-2, y+2*ydir, ydir, king, move_list);
		}
	}
	if ((x<6 && (y+2*ydir>=0 && y+2*ydir<=8)) && ((board_struct->board[y+ydir][x+1] == XCHECK) || (board_struct->board[y+ydir][x+1] == XKING))) {
		if (board_struct->board[y+2*ydir][x+2] == 0) {
			printf("%d, %d\n", x+2, y+2*ydir);
			printcaptures(board_struct, x+2, y+2*ydir, ydir, king, move_list);
		}
	}

	if (king) {
		printf("King captures");
		if (x>1 && ((board_struct->board[y-ydir][x-1] == XCHECK) || (board_struct->board[y-ydir][x-1] == XKING))) {
			if (board_struct->board[y-2*ydir][x-2] == 0) {
				printf("%d, %d\n", x-2, y-2*ydir);
			}
		}
		if (x<6 && ((board_struct->board[y-ydir][x+1] == XCHECK) || (board_struct->board[y-ydir][x+1] == XKING))) {
			if (board_struct->board[y-2*ydir][x+2] == 0) {
				printf("%d, %d\n", x-2, y-2*ydir);
			}
		}
	}
}

struct LinkedList *printmoves(struct gamestate *board_struct, int x, int y) {
	struct LinkedList *move_list = malloc(sizeof(struct LinkedList));
	move_list->next = NULL;
	move_list->move = NULL;

	int ydir;
	int king;
	int type = board_struct->board[y][x];

	if (type == CHECK) {
		printf("Checker\n");
		ydir = -1;
		king = false;
	} else if (type == KING) {
		printf("King\n");
		ydir = -1;
		king = true;
	} else if (type == XCHECK) {
		printf("Enemy checker\n");
		ydir = 1;
		king = false;
	} else if (type == XKING) {
		printf("Enemy king\n");
		ydir = 1;
		king = true;
	} else {
		printf("No checker here!\n");
		exit(1);
	}
	printf("Ydir: %d\n", ydir);
	if (((y+ydir) >= 0) && ((y+ydir) <= 7)) {
		if ((x > 0) && (board_struct->board[y+ydir][x-1] == 0)) {
			printf("Left: %d, %d\n", x-1, y+ydir);
			struct gamestate *new_move = malloc(sizeof(struct gamestate));
			memcpy(new_move, board_struct, sizeof(struct gamestate));
			new_move->board[y+ydir][x-1] = new_move->board[y][x];
			new_move->board[y][x] = 0;
			new_move->curr_x = x-1;
			new_move->curr_y = y+ydir;
			append(move_list, new_move);
		}
		if ((x < 7) && (board_struct->board[y+ydir][x+1] == 0)) {
			printf("Right: %d, %d\n", x+1, y+ydir);
			struct gamestate *new_move = malloc(sizeof(struct gamestate));
			memcpy(new_move, board_struct, sizeof(struct gamestate));
			new_move->board[y+ydir][x+1] = new_move->board[y][x];
			new_move->board[y][x] = 0;
			new_move->curr_x = x+1;
			new_move->curr_y = y+ydir;
			append(move_list, new_move);
		}
	}
	
	if (king) {
		if (((y-ydir) >= 0) && ((y-ydir) <= 7)) {
			if ((x > 0) && (board_struct->board[y-ydir][x-1] == 0)) {
				printf("%d, %d\n", x-1, y-ydir);
				struct gamestate *new_move = malloc(sizeof(struct gamestate));
				memcpy(new_move, board_struct, sizeof(struct gamestate));
				new_move->board[y-ydir][x-1] = new_move->board[y][x];
				new_move->board[y][x] = 0;
				append(move_list, new_move);
			}
			if ((x < 7) && (board_struct->board[y-ydir][x+1] == 0)) {
				printf("%d, %d\n", x+1, y-ydir);
				struct gamestate *new_move = malloc(sizeof(struct gamestate));
				memcpy(new_move, board_struct, sizeof(struct gamestate));
				new_move->board[y-ydir][x+1] = new_move->board[y][x];
				new_move->board[y][x] = 0;
				append(move_list, new_move);
			}
		}
	}

	printcaptures(board_struct, x, y, ydir, king, move_list);

	return move_list;
}
	




int main() {

	int x, y;
	struct gamestate *board_struct = (struct gamestate*) malloc(sizeof(struct gamestate));

	for (int x=0; x<8; x++) {
		for (int y=0; y<8; y++) {
			if (y<=2) {
				if ((y%2 == 0) != (x%2 == 0)) {
					board_struct->board[y][x] = XCHECK;
				}
			} else if (y>=5) {
				if ((y%2 == 0) != (x%2 == 0)) {
					board_struct->board[y][x] = CHECK;
				}
			}
		}
	}
	board_struct->board[4][1] = XCHECK;
	board_struct->board[2][1] = XCHECK;
	board_struct->board[1][2] = 0;

	x = 2;
	y = 5;
	
	struct LinkedList *move_list;
	move_list = printmoves(board_struct, x, y);

	printf("Printing from list:\n");
	if (move_list->next) {
		move_list = move_list->next;
	}
	while (move_list->next) {
		printf("%d, %d\n", move_list->move->curr_x, move_list->move->curr_y);
		move_list = move_list->next;
	}

	return 0;
}
