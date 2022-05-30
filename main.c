#include "checkmove.h"
#include "board.h" 
#include "minimax.h"

void step(struct gamestate *game){
	
	printboard(game->board);
	//line after board = 10
	int line = 10;
	int count = 0;
	//The maximum checkers you can have is 12.
	struct Space checks[12];
	struct Space kings[12];
	mvprintw(line, 0, "%s", "What would you like to move?");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++){
			if (game->board[i][j] == CHECK){
				//the first possible choice should print as "1."
				struct Space check = { j, i };
				checks[count] = check;
				count++;
				mvprintw(line+count, 0, "%d. Check at (%d, %d).", count, j, i);
			} else if (game->board[i][j] == KING) {
				struct Space king = { j, i };
				kings[count] = king;	
				count++;
				mvprintw(line+count, 0, "%d. King at (%d, %d).", count, j, i);
			}
		}
	}
	count++;
	mvprintw(line+count, 0, "%s", "Please choose a number from the list. ");
	
	int choice;
	int done = 0;
	int xval = 0;
	int yval = 0;

	struct LinkedList *list = malloc(sizeof(struct LinkedList));
	while (!done) {
		scanw("%d", &choice);
		
		choice--;
		//Accounts for the first Space in checks being stored at 0
		//while the first choice is printed as 1.

		xval = checks[choice].x;
		yval = checks[choice].y;
		//not yet compatible with kings, can fix if we don't cut kings
		
		//debug lines
		//count++;
		//mvprintw(line+count, 0, "%d, %d", xval, yval);

		list = getmoves(game, xval, yval);

		if (!list->next){ // skip sentinel node
			count++;
			mvprintw(line+count, 0, "%s", "Sorry, this cannot go anywhere. Please pick another.");
		} else {
			done = 1;
		}	
	}
	for (int i = 0; i <= count; i++){
		move(10+i, 0);
		clrtoeol();
	}	

	count = 0;
	int listint=1;
	mvprintw(line+count, 0, "You can move this piece (%d, %d) to:", xval, yval);	
	list = list->next; //skip sentinel node

	while (list->move) {
		count++;
		struct gamestate *future = list->move;
		mvprintw(line+count, 0, "%d. (%d, %d).", listint, future->curr_x, future->curr_y);
		listint++;
		if (list->next){
			list = list->next;
		} else {
			break;
		}
	}	
	scanw("%d", &choice);
	clear();
	//update board with player turn then opponent turn
	list = getmoves(game, xval, yval);
	list = list->next;
	for (int i = 1; i < choice; i++) {
		list = list->next;
	}
	printboard(list->move->board);
	//loop until completion
	getch();
	//wait
	//opponent

	freeLinkedList(list);

	//here for testing purposes only:
	//should only execute when gameover
	endwin();
	exit(0); 
}

void initPiece(struct Piece *p, struct gamestate *b, int val, int x, int y, int isPlayer) {
	p->value = val;
	p->coords.x = x; 
	p->coords.y = y;

	if (isPlayer) {
		static int p_idx = 0;
		b->player_pieces[p_idx] = p;
		p_idx++;
	} else {
		static int ai_idx = 0;
		b->ai_pieces[ai_idx] = p;
		ai_idx++;
	}
}

void initGame(struct gamestate *b) {
	(*b).turn = 0; // player go 1st for now
	(*b).score = 0;

	// initialize board
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++) {
			if (i <= 2) { // AI pieces
				if ((i == 0 || i == 2)) {
					if (j % 2 != 0) {
						(*b).board[i][j] = XCHECK; //global var
						struct Piece *p = malloc(sizeof(struct Piece));
						initPiece(p, b, XCHECK, i, j, 0);
					} else {
						(*b).board[i][j] = EMPTY; 
					}
				} else {
					if (j % 2 == 0) {
						(*b).board[i][j] = XCHECK; //global var
						struct Piece *p = malloc(sizeof(struct Piece));
                                                initPiece(p, b, XCHECK, i, j, 0);
					} else {
						(*b).board[i][j] = EMPTY; 
					}
				}
			}
			if (i >= 5) { // player pieces
				if ((i == 5 || i == 7)) {
					if (j % 2 == 0) {
						(*b).board[i][j] = CHECK; //global var
						struct Piece *p = malloc(sizeof(struct Piece));
                                                initPiece(p, b, CHECK, i, j, 1);
					} else {
						(*b).board[i][j] = EMPTY; 
					}
				} else {
					if (j % 2 != 0) {
						(*b).board[i][j] = CHECK; //global var
						struct Piece *p = malloc(sizeof(struct Piece));
                                                initPiece(p, b, CHECK, i, j, 1);
					} else {
						(*b).board[i][j] = EMPTY; 
					}
				}
			}
		}
	}
}

void freegame(struct gamestate *game) {
	for (int i=0; i<12; i++) {
		free(game->player_pieces[i]);
		free(game->ai_pieces[i]);
	}
}

int main() {
	initscr();
        if(has_colors() == FALSE) {
                endwin();
                printf("Your terminal does not support color.\n");
                exit(1);
        }

	start_color();

	struct gamestate *game = malloc (sizeof(struct gamestate)); 
	// will be initialized to default checkerboard
	
	initGame(game);
	
	step(game); //prints board, passes gamestate
	//depending on turn #, changes the order of player turn and opponent turn
	
	endwin();
	freegame(game);
	free(game);
	return 0;
}
