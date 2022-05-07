#include "board.h"

// put global variables here

void step(int gamestate[]){
	
	printboard(gamestate);
	//line after board, 
	mvprintw(10, 0, "%s", "What would you like to do? You may:");
	//for choice in choices, print "%num: Move %check to %square")
	getch();
	//choices shouuld be listed with numbers
	//after player chooses, destroy the extra prints
	//update board with player turn then opponent turn
	//loop until completion
	endwin();
	exit(0);
}

struct gamestate {
	int board[8][8];
	int turn;
	int score;
};

void initGame(struct board *b) {
	(*b).turn = 0; // player go 1st for now
	(*b).score = 0;

	// initialize board
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++) {
			if (i <= 2) { // AI pieces
				if ((i == 0 || i == 2)) {
					if (j % 2 != 0) {
						(*b).board[i][j] = _; //global var
					} else {
						(*b).board[i][j] = EMPTY; 
					}
				} else {
					if (j % 2 == 0) {
						(*b).board[i][j] = _; //global var
					} else {
						(*b).board[i][j] = EMPTY; 
					}
				}
			}
			if (i >= 5) { // player pieces
				if ((i == 5 || i == 7)) {
					if (j % 2 == 0) {
						(*b).board[i][j] = _; //global var
					} else {
						(*b).board[i][j] = EMPTY; 
					}
				} else {
					if (j % 2 != 0) {
						(*b).board[i][j] = _; //global var
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

	int gamestate[64]; // will be initialized to default checkerboard
	
	int line = -1;	
	for (int i = 0; i < 64; i++){
		if (i%8 == 0){
                        line++;
                }
		if ((line > 2) && (line < 5)) {
			gamestate[i] = EMPTY;
		}
		else if (line%2 == 0){
			if (i%2 == 0) {	
				gamestate[i] = EMPTY;
			} else {
				if (64/i >= 2) {
					gamestate[i] = XCHECK;
				} else {
					gamestate[i] = CHECK;
				}
			}
		} else {
			if (i%2 == 0) {
				if (64/i >= 2) {
					gamestate[i] = XCHECK;
				} else {
					gamestate[i] = CHECK;
				}
			} else {
				gamestate[i] = EMPTY;
			}
		}
	}

	step(gamestate); //prints board, passes gamestate
	//depending on turn #, changes the order of player turn and opponent turn
	
//	endwin();
}
