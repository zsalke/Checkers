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
//



// checks if 1 of the players has reached the end of
// the board w/ all their pieces
int checkEnd(struct gamestate *game) {
	int p_idx = 0;
	int num_playerpieces = 0;
        int pp_at_end = 0;
	while (game->player_pieces[p_idx]) {
		num_playerpieces++;
		if (game->player_pieces[p_idx]->coords.y == 0) {
			pp_at_end++;
		}
		p_idx++;
	}
	
	int ai_idx = 0;
	int num_aipieces = 0;
	int ai_at_end = 0;
        while(game->ai_pieces[ai_idx]) {
                num_aipieces++;
                if (game->ai_pieces[ai_idx]->coords.y == 7) { 
			ai_at_end++;
                } 
		ai_idx++;
        }
	

	if (num_playerpieces == pp_at_end || num_aipieces == ai_at_end) {
		return 1;
	}
	return 0;
}

// returns who won when checkEnd
void checkWin(struct gamestate *game) {
	if (game->score == 0) {
		mvprintw(10, 50, "Tie!");
		getch();
		endwin();
		exit(0);
	}
	else if (game->score < 0) {
		mvprintw(10, 50, "You win!");
		getch();
		endwin();
		exit(0);
	}
	else {
		mvprintw(10, 50, "You lose!");
		getch();
		endwin();
		exit(0);
	}
}




// List operations
//

void initPiece(struct Piece *p, struct gamestate *b, int val, int x, int y, int isPlayer) {
	p->value = val;
	p->coords.x = x; 
	p->coords.y = y;
	
	if (isPlayer) {
		int p_idx = 0;
		while (b->player_pieces[p_idx]) {
			p_idx++;
		}

		b->player_pieces[p_idx] = p;
	} else {
		int ai_idx = 0;
		while(b->ai_pieces[ai_idx]) {
			ai_idx++;
		}
		b->ai_pieces[ai_idx] = p;
	}
}



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
	}
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

	// Free arrays before updating
	
	for (int i=0; i<12; i++) {
		/*
		if (new_move->player_pieces[i]) {
			free(new_move->player_pieces[i]);
		}
		if (new_move->ai_pieces[i]) {
			free(new_move->ai_pieces[i]);
		}
*/
		new_move->player_pieces[i] = NULL;
		new_move->ai_pieces[i] = NULL;
	}


	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (new_move->board[i][j] != EMPTY) {
				struct Piece *p = malloc(sizeof(struct Piece));
				
				// AI's piece
				if (new_move->board[i][j] == 3 || new_move->board[i][j] == 4) {
					initPiece(p, new_move, new_move->board[i][j], j, i, 0);
				}
				else if (new_move->board[i][j] == 1 || new_move->board[i][j] == 2) { // player's piece
					initPiece(p, new_move, new_move->board[i][j], j, i, 1);
				}
			}
		}
	}
	
}

void printcaptures(struct gamestate *board_struct, int x, int y, int ydir, bool isking, struct LinkedList *move_list, bool isPlayer) {
//	printf("Print captures\n");
	
	int xcheck, xking;
	if (isPlayer) {
		xcheck = XCHECK;
		xking = XKING;
	} else {
		xcheck = CHECK;
		xking = KING;
	}

	if ((x>1 && (y+2*ydir>=0 && y+2*ydir<=7)) && ((board_struct->board[y+ydir][x-1] == xcheck) || (board_struct->board[y+ydir][x-1] == xking))) {
		if (board_struct->board[y+2*ydir][x-2] == 0) {
//			printf("%d, %d\n", x-2, y+2*ydir);
			struct gamestate *new_move = malloc(sizeof(struct gamestate));
			memcpy(new_move, board_struct, sizeof(struct gamestate));
			setmove(new_move, x, y, x-2, y+2*ydir);
			new_move->board[y+ydir][x-1] = 0; // remove captured piece
			updatePieceArrays(new_move);
			append(move_list, new_move);
//			if (isOpp) {
//			}

			printcaptures(new_move, x-2, y+2*ydir, ydir, isking, move_list, isPlayer);
		}
	}
	if ((x<6 && (y+2*ydir>=0 && y+2*ydir<=7)) && ((board_struct->board[y+ydir][x+1] == xcheck) || (board_struct->board[y+ydir][x+1] == xking))) {
		if (board_struct->board[y+2*ydir][x+2] == 0) {
//			printf("%d, %d\n", x+2, y+2*ydir);

			struct gamestate *new_move = malloc(sizeof(struct gamestate));
			memcpy(new_move, board_struct, sizeof(struct gamestate));
			setmove(new_move, x, y, x+2, y+2*ydir);
			new_move->board[y+ydir][x+1] = 0; // remove captured piece
			updatePieceArrays(new_move);
			append(move_list, new_move);
//			if (isOpp) {
//				mvprintw(0, 0, "Opponent capture");
//			}

			printcaptures(new_move, x+2, y+2*ydir, ydir, isking, move_list, isPlayer);
		}
	}

	if (isking) {
		printf("King captures");
		if (x>1 && ((board_struct->board[y-ydir][x-1] == xcheck) || (board_struct->board[y-ydir][x-1] == xking))) {
			if (board_struct->board[y-2*ydir][x-2] == 0) {
//				printf("%d, %d\n", x-2, y-2*ydir);

				struct gamestate *new_move = malloc(sizeof(struct gamestate));
				memcpy(new_move, board_struct, sizeof(struct gamestate));
				setmove(new_move, x, y, x+2, y-2*ydir);
				new_move->board[y-ydir][x+1] = 0;
				updatePieceArrays(new_move);
				append(move_list, new_move);

				printcaptures(new_move, x+2, y+2*ydir, ydir, isking, move_list, isPlayer);
			}
		}
		if (x<6 && ((board_struct->board[y-ydir][x+1] == xcheck) || (board_struct->board[y-ydir][x+1] == xking))) {
			if (board_struct->board[y-2*ydir][x+2] == 0) {
//				printf("%d, %d\n", x-2, y-2*ydir);

				struct gamestate *new_move = malloc(sizeof(struct gamestate));
				memcpy(new_move, board_struct, sizeof(struct gamestate));
				setmove(new_move, x, y, x-2, y-2*ydir);
				new_move->board[y-ydir][x-1] = 0;
				updatePieceArrays(new_move);
				append(move_list, new_move);

				printcaptures(new_move, x+2, y+2*ydir, ydir, isking, move_list, isPlayer);
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
	bool isking, isPlayer;
	int type = board_struct->board[y][x];

	int check, king, xcheck, xking;
	if (board_struct->turn % 2 == 0) {
		isPlayer = true;
		check = CHECK;
		king = KING;
		xcheck = XCHECK;
		xking = XKING;
	} else {
		isPlayer = false;
		check = XCHECK;
		king = XKING;
		xcheck = CHECK;
		xking = KING;
	}


	if (type == check) {
//		printf("Checker\n");
		ydir = -1;
		isking = false;

		isPlayer = true;
	} else if (type == king) {
//		printf("King\n");
		ydir = -1;
		isking = true;

		isPlayer = true;
	} else if (type == xcheck) {
//		printf("Enemy checker\n");
		ydir = 1;
		isking = false;

		isPlayer = false;
	} else if (type == xking) {
//		printf("Enemy king\n");
		ydir = 1;
		isking = true;

		isPlayer = false;
	} else {
//		printf("No checker here!\n");
		return move_list;
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
	
	if (isking) {
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

	printcaptures(board_struct, x, y, ydir, isking, move_list, isPlayer);

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
                        if (game->ai_pieces[i]) {

                        	struct Piece *p = game->ai_pieces[i];
                                struct LinkedList *piecemoves = getmoves(game, p->coords.x, p->coords.y); // possible moves for 1 piece
				appendList(allmoves, piecemoves);
                        }
                }
	}
/*
	bool hasmove = false;
	struct MovesLists *iterator = allmoves;

	while (iterator->next) {
		iterator = iterator->next;
		if (iterator->list->next) {
			hasmove = true;
		}
	}

	if (hasmove == false) {
		checkWin(game);
	}	
*/
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
