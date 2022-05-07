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


void printboard(int [8][8]);
struct board;
struct space;


#endif
