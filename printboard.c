#include "board.h"

void printboard(int gamestate[]){
	//screen should already be init in main

	int count = 0;
	
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	//opponent
	init_pair(2, COLOR_RED, COLOR_BLACK);

	int line = 0;
	for (int i = 0; i < BOARD_SIZE; i++){
		mvaddch(line+1, 2*count, '|');
		if (gamestate[i] == CHECK){
			attron(COLOR_PAIR(1));
			addch('c');
			attroff(COLOR_PAIR(1));
		} else if (gamestate[i] == KING) {
			attron(COLOR_PAIR(1));
			addch('C');
			attroff(COLOR_PAIR(1));
		} else if (gamestate[i] == XCHECK) {
			attron(COLOR_PAIR(2));
			addch('c');
			attroff(COLOR_PAIR(2));
		} else if (gamestate[i] == XKING) {
			attron(COLOR_PAIR(2));
			addch('C');
			attroff(COLOR_PAIR(2));
		} else {
			addch(' ');
		}
		count++;
		if (count%8 == 0) {
			addch('|');
			line++;
			count = 0;
		}
	}
	
	refresh();
	//getch();
	//endwin();
	//
}

/*int main(){
	int board[BOARD_SIZE];
	for (int i = 0; i < BOARD_SIZE; i++){
		if (i%2 == 0){
		 board[i] = 0;
		} else if ((BOARD_SIZE/i) < 2) {
		 board[i] = 1;
		} else {
		 board[i] = 3;
		}
	}
	printboard(board);
}*/
