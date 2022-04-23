#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define BOARD_SIZE 16
#define EMPTY 0
#define CHECK 1
#define KING 2
#define XCHECK 3
#define XKING 4
void printboard(int gamestate[]){
	int count = 0;
	
	initscr();
	if(has_colors() == FALSE){
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}	
	start_color();
	
	//self
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	//opponent
	init_pair(2, COLOR_RED, COLOR_BLACK);

	int row,col;

	getmaxyx(stdscr, row, col);

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
			mvhline(line, 0, '_',16);
			line+=2;
			count = 0;
		}
	}
	
	refresh();
	getch();
	endwin();
}

int main(){
	int arr[17] = {1, 0, 1, 0, 2, 0, 4, 0, 0, 1, 0, 1, 0, 3, 0, 3};
	printboard(arr);
}
