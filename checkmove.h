#include <stdio.h>
#include <string.h>
#include "board.h"

#ifndef HEADER_FILE
#define HEADER_FILE

struct LinkedList;
struct MovesLists;

void append(struct LinkedList, struct gamestate);
void appendMovesLists(struct MovesLists, struct LinkedList);
void setmove(struct gamestate, int prev_x, int prev_y, int curr_x, int curr_y);
void printcaptures(struct gamestate, int x, int ydir, bool king, struct LinkedList);
struct LinkedList *printmoves(struct gamestate, int x, int y);
struct LinkedList *getAllmoves(struct gamestate, int isPlayer);
