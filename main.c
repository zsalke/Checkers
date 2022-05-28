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
	//The maximum checkers you can have is 12.
	struct Space checks[12];
	struct Space kings[12];
	mvprintw(line, 0, "%s", "What would you like to move?");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++){
			if (game->board[i][j] == CHECK){
				//the first possible choice should print as "1."
				struct Space ij = { i, j };
				checks[count] = ij;
				count++;
				mvprintw(line+count, 0, "%d. Check at (%d, %d).", count, i, j);
			} else if (game->board[i][j] == KING) {
				struct Space ij = { i, j };
				kings[count] = ij;	
				count++;
				mvprintw(line+count, 0, "%d. King at (%d, %d).", count, i, j);
			}
		}
	}
	count++;
	mvprintw(line+count, 0, "%s", "Please choose a number from the list. ");

	int choice = getch()-49;
	/* the first valid checker is stored at checks[0], but
	 * is listed as 1. to the player. hence, shifting the
	 * player's choice to be 1 less will remove the error. 
	 */
	int xval = checks[choice].x;
	int yval = checks[choice].y;
	//not yet compatible with kings, can fix if we don't cut kings
	count++;
	//mvprintw(line+count, 0, "%d, %d", xval, yval);
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

	struct gamestate *game = malloc (sizeof(struct gamestate)); 
	// will be initialized to default checkerboard
	
	initGame(game);

	step(game); //prints board, passes gamestate
	//depending on turn #, changes the order of player turn and opponent turn
	
//	endwin();
}
