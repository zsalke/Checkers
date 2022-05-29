#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define BOARD_SIZE 64
#define EMPTY 0
#define CHECK 1
#define KING 2
#define XCHECK 3
#define XKING 4

#ifndef HEADER_FILE
#define HEADER_FILE

struct Space {
        int x;
        int y;
};

struct piece {
	int value; // either CHECK or KING
	struct Space coords; // its i & j in board
};


struct gamestate {
        int board[8][8];
        int turn;
        int score;
	int prev_x;
	int prev_y;
	int curr_x;
	int curr_y;

	struct piece player_pieces[12]; // starts w/ 12
	struct piece ai_pieces[12];
	struct gamestate *parent; // for AI predicting move purposes
};



void printboard(int [8][8]);
struct board;
#endif
