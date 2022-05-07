#include "board.h"

// put global variables here

struct gamestate {
        int board[8][8];
        int turn;
        int score;
};

void step(struct gamestate *game){
	
	printboard(game->board);
	//line after board, 
	mvprintw(10, 0, "%s", "What would you like to do? You may:");
	//for choice in choices, print "%num: Move %check to %square")
	int choiceslen = 1;
	getch();
	//choices shouuld be listed with numbers
	//after player chooses, destroy the extra prints
	for (int i = 0; i < choiceslen; i++) {
		move(10+i, 0);
		clrtoeol();
	}
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
