// checkmove.c
//

#include <stdio.h>
#include <string.h>
#include "board.h"
/*
struct gamestate {
        int board[8][8];
        int turn;
        int score;
	int prev_x;
	int prev_y;
	int curr_x;
	int curr_y;
};

struct LinkedList {
	struct gamestate *move;
	struct LinkedList *next;
};
*/
// a struct for all possible moves for all pieces (of 1 player)
//struct MovesLists {
//	struct LinkedList *list;
//	struct MovesLists *next;
//};

// List operations
void append(struct LinkedList *list, struct gamestate *new) {
	while (list->next) {
		list = list->next;
	}
	list->next = malloc(sizeof(struct LinkedList));
	list->next->move = new;
	list->next->next = NULL;
}

void freeLinkedList(struct LinkedList *head) {
	struct LinkedList *buff;
	while (head->next) {
		buff = head->next;
		free(head);
		head = buff;
	};
	free(head);
}	

void appendList(struct MovesLists *lists, struct LinkedList *new) {
        while (lists->next) {
                lists = lists->next;
        }
        lists->next = malloc(sizeof(struct MovesLists));
        lists->next->list = new;
        lists->next->next = NULL;
}


void freeMovesLists(struct MovesLists *head) {
	struct MovesLists *buff;
	while (head->next) {
		buff = head->next;
		free(head);
		head = buff;
	};
	free(head);
}	



// Abstraction to set variables in a gamestate
void setmove(struct gamestate *move, int prev_x, int prev_y, int curr_x, int curr_y) {
			move->board[curr_y][curr_x] = move->board[prev_y][prev_x];
			move->board[prev_y][prev_x] = 0;
			move->curr_x = curr_x;
			move->curr_y = curr_y;
			move->prev_x = prev_x;
			move->prev_y = prev_y;
}

// goes thru board & update piece arrays
void updatePieceArrays(struct gamestate *new_move) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (new_move->board[i][j] != EMPTY) {
				struct Piece *p = malloc(sizeof(struct Piece));
				
				// AI's piece
				if (new_move->board[i][j] == 3 || new_move->board[i][j] == 4) {
					initPiece(p, new_move, new_move->board[i][j], i, j, 0);
				}
				else { // player's piece
					initPiece(p, new_move, new_move->board[i][j], i, j, 1);
				}	
			}
		}
	}
}

// 
void printcaptures(struct gamestate *board_struct, int x, int y, int ydir, bool king, struct LinkedList *move_list) {
//	printf("Print captures\n");
	if ((x>1 && (y+2*ydir>=0 && y+2*ydir<=8)) && ((board_struct->board[y+ydir][x-1] == XCHECK) || (board_struct->board[y+ydir][x-1] == XKING))) {
		if (board_struct->board[y+2*ydir][x-2] == 0) {
//			printf("%d, %d\n", x-2, y+2*ydir);
			struct gamestate *new_move = malloc(sizeof(struct gamestate));
			memcpy(new_move, board_struct, sizeof(struct gamestate));
			setmove(new_move, x, y, x-2, y+2*ydir);
			new_move->board[y+ydir][x-1] = 0; // captured piece
			// remove captured piece from piece array
			updatePieceArrays(new_move);
			append(move_list, new_move);
			
			printcaptures(new_move, x-2, y+2*ydir, ydir, king, move_list);
		}
	}
	if ((x<6 && (y+2*ydir>=0 && y+2*ydir<=8)) && ((board_struct->board[y+ydir][x+1] == XCHECK) || (board_struct->board[y+ydir][x+1] == XKING))) {
		if (board_struct->board[y+2*ydir][x+2] == 0) {
//			printf("%d, %d\n", x+2, y+2*ydir);

			struct gamestate *new_move = malloc(sizeof(struct gamestate));
			memcpy(new_move, board_struct, sizeof(struct gamestate));
			setmove(new_move, x, y, x+2, y+2*ydir);
			new_move->board[y+ydir][x+1] = 0;
			updatePieceArrays(new_move);
			append(move_list, new_move);

			printcaptures(new_move, x+2, y+2*ydir, ydir, king, move_list);
		}
	}

	if (king) {
		printf("King captures");
		if (x>1 && ((board_struct->board[y-ydir][x-1] == XCHECK) || (board_struct->board[y-ydir][x-1] == XKING))) {
			if (board_struct->board[y-2*ydir][x-2] == 0) {
				printf("%d, %d\n", x-2, y-2*ydir);

				struct gamestate *new_move = malloc(sizeof(struct gamestate));
				memcpy(new_move, board_struct, sizeof(struct gamestate));
				setmove(new_move, x, y, x+2, y-2*ydir);
				new_move->board[y-ydir][x+1] = 0;
				updatePieceArrays(new_move);
				append(move_list, new_move);

				printcaptures(new_move, x+2, y+2*ydir, ydir, king, move_list);
			}
		}
		if (x<6 && ((board_struct->board[y-ydir][x+1] == XCHECK) || (board_struct->board[y-ydir][x+1] == XKING))) {
			if (board_struct->board[y-2*ydir][x+2] == 0) {
				printf("%d, %d\n", x-2, y-2*ydir);

				struct gamestate *new_move = malloc(sizeof(struct gamestate));
				memcpy(new_move, board_struct, sizeof(struct gamestate));
				setmove(new_move, x, y, x-2, y-2*ydir);
				new_move->board[y-ydir][x-1] = 0;
				updatePieceArrays(new_move);
				append(move_list, new_move);

				printcaptures(new_move, x+2, y+2*ydir, ydir, king, move_list);
			}
		}
	}
}

// Return a list of available moves for a given piece in a board state
struct LinkedList *getmoves(struct gamestate *board_struct, int x, int y) {

	struct LinkedList *move_list = malloc(sizeof(struct LinkedList));
	move_list->next = NULL;
	move_list->move = NULL;
//	printf("%p", board_struct);
	int ydir;
	int king;
	int type = board_struct->board[y][x];

	if (type == CHECK) {
//		printf("Checker\n");
		ydir = -1;
		king = false;
	} else if (type == KING) {
//		printf("King\n");
		ydir = -1;
		king = true;
	} else if (type == XCHECK) {
//		printf("Enemy checker\n");
		ydir = 1;
		king = false;
	} else if (type == XKING) {
//		printf("Enemy king\n");
		ydir = 1;
		king = true;
	} else {
//		printf("No checker here!\n");
		exit(1);
	}

//	printf("Ydir: %d\n", ydir);
	if (((y+ydir) >= 0) && ((y+ydir) <= 7)) {
		if ((x > 0) && (board_struct->board[y+ydir][x-1] == 0)) {
//			printf("Left: %d, %d\n", x-1, y+ydir);
			struct gamestate *new_move = malloc(sizeof(struct gamestate));
			memcpy(new_move, board_struct, sizeof(struct gamestate));
			setmove(new_move, x, y, x-1, y+ydir);
			append(move_list, new_move);
		}
		if ((x < 7) && (board_struct->board[y+ydir][x+1] == 0)) {
//			printf("Right: %d, %d\n", x+1, y+ydir);
			struct gamestate *new_move = malloc(sizeof(struct gamestate));
			memcpy(new_move, board_struct, sizeof(struct gamestate));

			setmove(new_move, x, y, x+1, y+ydir);
			append(move_list, new_move);
		}
	}
	
	if (king) {
		if (((y-ydir) >= 0) && ((y-ydir) <= 7)) {
			if ((x > 0) && (board_struct->board[y-ydir][x-1] == 0)) {
//				printf("%d, %d\n", x-1, y-ydir);
				struct gamestate *new_move = malloc(sizeof(struct gamestate));
				memcpy(new_move, board_struct, sizeof(struct gamestate));
				new_move->board[y-ydir][x-1] = new_move->board[y][x];
				new_move->board[y][x] = 0;
				append(move_list, new_move);
			}
			if ((x < 7) && (board_struct->board[y-ydir][x+1] == 0)) {
//				printf("%d, %d\n", x+1, y-ydir);
				struct gamestate *new_move = malloc(sizeof(struct gamestate));
				memcpy(new_move, board_struct, sizeof(struct gamestate));
				new_move->board[y-ydir][x+1] = new_move->board[y][x];
				new_move->board[y][x] = 0;
				append(move_list, new_move);
			}
		}
	}

	printcaptures(board_struct, x, y, ydir, king, move_list);

	// Hacky fix for captures
	// In order to fix the origin of moves with multiple captures, reset all prev_x and prev_y values to original x and y
	struct LinkedList *iterator = move_list;
	while (iterator->next) {
		iterator = iterator->next;
		iterator->move->prev_x = x;
		iterator->move->prev_y = y;
	}

	return move_list;
}
	
// returns a LinkedList of LinkedLists of all possible moves
// based on the given player (pass in gamestate->turn % 2)
struct MovesLists *getAllmoves(struct gamestate *game, int isPlayer) {
	struct MovesLists *allmoves = malloc(sizeof(struct MovesLists));
	allmoves->next = NULL;
        allmoves->list = NULL;

	if (isPlayer == 0) { // player's turn
		for (int i = 0; i < 12; i++) {
			struct Piece *p = game->player_pieces[i];
			if (p) {
				struct LinkedList *piecemoves = getmoves(game, p->coords.x, p->coords.y); // possible moves for 1 piece
				appendList(allmoves, piecemoves);
			}
		}
	}
	else { // isAI
		for (int i = 0; i < 12; i++) {
                        struct Piece *p = game->ai_pieces[i];
                        if (p) {
                                struct LinkedList *piecemoves = getmoves(game, p->coords.x, p->coords.y); // possible moves for 1 piece
                                appendList(allmoves, piecemoves);
                        }
                }
	}

	return allmoves;
}


/*
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
	move_list = getmoves(board_struct, x, y);

	printf("Printing from list:\n");
	if (move_list->next) {
		move_list = move_list->next;
	}
	while (move_list->next) {
		printf("%d, %d\n", move_list->move->curr_x, move_list->move->curr_y);
		move_list = move_list->next;
	}

	printf("%d, %d\n", move_list->move->curr_x, move_list->move->curr_y);

	return 0;
}
*/
