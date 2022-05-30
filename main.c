#include "checkmove.h"
#include "board.h" 

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
	scanw("%d", &choice);
	choice--;
	//Accounts for the first Space in checks being stored at 0
	//while the first choice is printed as 1.
	
	int xval = checks[choice].x;
	int yval = checks[choice].y;
	//not yet compatible with kings, can fix if we don't cut kings
	count++;
	mvprintw(line+count, 0, "%d, %d", xval, yval);
	
	struct LinkedList *list;

	list = getmoves(game, xval, yval);
		

	//struct gamestate futures[] = checkmove(game, xval, yval);	
	//check gamestates[i] presentcoords variable
	//and check if pastcoords match xval/yval in case recursivity does weird shit
	//print that present as a possibility
	//have player make selection
	//if checkmove produces no possibilities, run choice again. maybe put in while loop
	//until checkmove gives them something?
	//do the board thing
	getch();	
	clear();
	//update board with player turn then opponent turn
	//loop until completion
	
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
