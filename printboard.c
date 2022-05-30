#include "board.h"

void printboard(int board[8][8]){
	//screen should already be init in main
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	//opponent
	init_pair(2, COLOR_RED, COLOR_BLACK);

	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
		//puts a | between each check
			mvaddch(i+1, 2*j, '|');
			if (board[i][j]  == CHECK){
				attron(COLOR_PAIR(1));
				addch('c');
				attroff(COLOR_PAIR(1));
			} else if (board[i][j] == KING) {
				attron(COLOR_PAIR(1));
				addch('C');
				attroff(COLOR_PAIR(1));
			} else if (board[i][j] == XCHECK) {
				attron(COLOR_PAIR(2));
				addch('c');
				attroff(COLOR_PAIR(2));
			} else if (board[i][j] == XKING) {
				attron(COLOR_PAIR(2));
				addch('C');
				attroff(COLOR_PAIR(2));
			} else {
				addch(' ');
			}
			addch('|');
		}
		addch(i+48);
	}
	refresh();
}
/* For independent testing purposes.
int main(){
	initscr();
	cbreak();
	start_color();
	int board[8][8];
	
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 8; j++) {
			if (i <= 2) { // AI pieces
				if ((i == 0 || i == 2)) {
					if (j % 2 != 0) {
						board[i][j] = XCHECK; //global var
					} else {
						board[i][j] = EMPTY; 
					}
				} else {
					if (j % 2 == 0) {
						board[i][j] = XCHECK; //global var
					} else {
						board[i][j] = EMPTY; 
					}
				}
			}
			if (i >= 5) { // player pieces
				if ((i == 5 || i == 7)) {
					if (j % 2 == 0) {
						board[i][j] = CHECK; //global var
					} else {
						board[i][j] = EMPTY; 
					}
				} else {
					if (j % 2 != 0) {
						board[i][j] = CHECK; //global var
					} else {
						board[i][j] = EMPTY; 
					}
				}
			}
		}
	}	
	printboard(board);
	getch();
	endwin();
}
*/
