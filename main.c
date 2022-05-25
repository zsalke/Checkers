#include "board.h"

// put global variables here

struct gamestate {
        int board[8][8];
        int turn;
        int score;
};

void step(struct gamestate *game){
	
	printboard(game->board);
	//line after board = 10
	int line = 10;
	int count = 0;
       	mvprintw(line, 0, "%s", "What would you like to move?");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++){
			if (game->board[i][j] == CHECK){
				//the first possible choice should print as "1."
				count++;
				mvprintw(line+count, 0, "%d. Check at (%d, %d).", count, i, j);
			} else if (game->board[i][j] == KING) {
				count++;
				mvprintw(line+count, 0, "%d. King at (%d, %d).", count, i, j);
			}
		}
	}	
	int choice = getch();
	int *xval;
	int *yval;
	//because of choices indexing from 1, this shouldn't reread the board/input prompt.
	mvscanw(line+(choice-48),3,"%d %d", &xval, &yval);
	mvprintw(line, 0, "%s", "What would you like to do? You may:");
	//for choice in choices, print "%num: Move %check to %square")
	struct gamestate futures[] = checkmove(game, *xval, *yval);	
	//check gamestates[i] presentcoords variable
	//and check if pastcoords match xval/yval in case recursivity does weird shit
	//print that present as a possibility
	//have player make selection
	//do the board thing
	
	getch();
	//choices shouuld be listed with numbers
	//after player chooses, destroy the extra prints
	clear();
	getch();
	//update board with player turn then opponent turn
	//loop until completion
	endwin();
	exit(0); 
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
					} else {
						(*b).board[i][j] = EMPTY; 
					}
				} else {
					if (j % 2 == 0) {
						(*b).board[i][j] = XCHECK; //global var
					} else {
						(*b).board[i][j] = EMPTY; 
					}
				}
			}
			if (i >= 5) { // player pieces
				if ((i == 5 || i == 7)) {
					if (j % 2 == 0) {
						(*b).board[i][j] = CHECK; //global var
					} else {
						(*b).board[i][j] = EMPTY; 
					}
				} else {
					if (j % 2 != 0) {
						(*b).board[i][j] = CHECK; //global var
					} else {
						(*b).board[i][j] = EMPTY; 
					}
				}
			}
		}
	}
}

int main() {
	initscr();
	cbreak(); //ncurses should get keyboard input directly
        if(has_colors() == FALSE) {
                endwin();
                printf("Your terminal does not support color.\n");
                exit(1);
        }

	start_color();

	struct gamestate *game; // will be initialized to default checkerboard
	
	initGame(game);

	step(game); //prints board, passes gamestate
	//depending on turn #, changes the order of player turn and opponent turn
	
//	endwin();
}
