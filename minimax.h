#include <stdio.h>
#include <string.h>
#include "board.h"
#include "checkmove.h"

#ifndef MINIMAX_HEADER
#define MINIMAX_HEADER

// move this to main or something
void changeTurn(struct gamestate *game);

int calc_score(struct gamestate *game); 

// finds board w/ min score to help AI get best move in reponse to player's best
// MovesLists are nodes themselves
struct gamestate *findMinGame(struct MovesLists *allpossible_moves); 

struct gamestate *findMaxGame(struct MovesLists *allpossible_moves);

// return init_possible_moves[best] (depth 1)
struct gamestate *findBestMove(struct gamestate *curr); 

// calculates scores for init_allpossible_moves (depth 3 moves)
// thinking 2 moves ahead
// parentGame is 1 of init_allpossible_moves
// this will return multiple results to append to LinkedList depth3scores
void findPossibleScores(struct gamestate *parentGame, struct gamestate *tmp, struct LinkedList *depth3scores); 


#endif
