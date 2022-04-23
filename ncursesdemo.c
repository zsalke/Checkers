#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

void printing();
void colouring();

int main() {

	int row, col;

	char mesg[] = "Lorem ipsum";

	initscr(); // starts ncurses
	
	getmaxyx(stdscr, row, col); //stores size of terminal window
	mvprintw(row/2,(col-strlen(mesg)-14)/2,"%s",mesg);

	//addstr("\ntest print str\n");
	//refresh; // flush the buffer to the screen that ncurses outputs

	//printing();
	colouring();
	
	addstr("\npress any key to exit...");
    	refresh();
	getch(); // so program doesn't terminate immediately

	endwin(); // ends ncurses

	return EXIT_SUCCESS;
}

void printing() {
        addch('c');
        refresh;

        printw("\nThis was printed with printw\n");
        refresh;
}

void colouring() {
	
	// have to check if terminal can print in color
	if (has_colors()) {
		if (start_color() == OK) {
			// takes a no. 1-64, foreground, background
			init_pair(1, COLOR_WHITE, COLOR_BLUE);

			attrset(COLOR_PAIR(1));
			addstr("Yellow and red\n\n");
            		refresh();
            		attroff(COLOR_PAIR(1));
		} else {
			addstr("Can't start colors\n");
			refresh();
		}
	} else {
		addstr("This terminal is not color capable\n");
		refresh();
	}
}
