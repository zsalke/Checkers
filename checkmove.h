#include <stdio.h>
#include <string.h>
#include "board.h"

#ifndef CHECKMOVE_HEADER
#define CHECKMOVE_HEADER
/*
struct LinkedList {
	struct gamestate *move;
	struct LinkedList *next;
};

struct MovesLists;
*/

void initPiece(struct Piece *p, struct gamestate *b, int val, int x, int y, int isPlayer);
void append(struct LinkedList*, struct gamestate*);
void appendMovesLists(struct MovesLists, struct LinkedList);
void setmove(struct gamestate*, int prev_x, int prev_y, int curr_x, int curr_y);
void printcaptures(struct gamestate*, int x, int ydir, bool king, struct LinkedList);
struct LinkedList *getmoves(struct gamestate*, int x, int y);
struct MovesLists *getAllmoves(struct gamestate*, int);
void freeLinkedList(struct LinkedList*);
void freeMovesLists(struct MovesLists*);



#endif
