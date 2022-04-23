#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define EMPTY 0
#define CHECK 1
#define KING 2

void printboard(int gamestate[]){
	int count = 0;
	
	initscr();
	start_color();

	int row,col;

	getmaxyx(stdscr, row, col);

	int line = 0;
	for (int i = 0; i < 16; i++){
		//mvaddch(line, 2*count+1, '_');
		mvaddch(line+1, 2*count, '|');
		if (gamestate[i] == CHECK){
			addch('c');
		} else if (gamestate[i] == KING) {
			addch('C');
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
	//printf("\n");
	getch();
	endwin();
}

int main(){
	int arr[17] = {1, 0, 1, 0, 2, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1};
	printboard(arr);
}
