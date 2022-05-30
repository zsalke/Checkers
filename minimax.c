#include <stdio.h>
#include <string.h>
#include "board.h"
#include "checkmove.h"

// move this to main or something
void changeTurn(struct gamestate *game) {
	if (game->turn % 2 == 0) { // player made a move
		game->turn = 1; // now AI's turn
	} else {
		game->turn = 2; //dk if we're incrementing or just assigning
	}
}
// 

int calc_score(struct gamestate *game) {
	int num_playerpieces = 0;
	int num_aipieces = 0;
	for (int i = 0; i < 12; i++) {
		if (game->player_pieces[i]->value != -1) {
			num_playerpieces++;
		}
		if (game->ai_pieces[i]->value != -1) {
                        num_aipieces++;
                }
	}

	return num_aipieces - num_playerpieces;
}

// finds board w/ min score to help AI get best move in reponse to player's best
// MovesLists are nodes themselves
struct gamestate *findMinGame(struct MovesLists *allpossible_moves) {
	struct LinkedList *piece_moves = allpossible_moves->next->list; // null head
	struct gamestate *minGame = piece_moves->next->move;

	while (piece_moves != NULL) {
		struct gamestate *gameptr = piece_moves->move;
		while (gameptr != NULL) {
			
			//NEED A UPDATE GAMESTATE FUNCTION ***
			//-wait checkmoves.c returns an updated board
			//(update ai_pieces, player_pieces, board, captures etc)
			
			gameptr->score = calc_score(gameptr);
			
			if (gameptr->score < minGame->score) {
				minGame = gameptr; //dk if work cuz &'s			
			}
			gameptr = gameptr->next;
		}
		piece_moves = piece_moves->next;
	}

	return minGame;
}

struct gamestate *findMaxGame(struct MovesLists *allpossible_moves) {
        struct LinkedList *piece_moves = allpossible_moves->next->list;
        struct gamestate *maxGame = piece_moves->next->move;

        while (piece_moves != NULL) {
                struct gamestate *gameptr = piece_moves->move;
                while (gameptr != NULL) {

                        //NEED A UPDATE GAMESTATE FUNCTION ***
                        //(update ai_pieces, player_pieces, board, captures etc)

                        gameptr->score = calc_score(gameptr);

                        if (gameptr->score > maxGame->score) {
                                maxGame = gameptr; //dk if work cuz &'s
                        }
                        gameptr = gameptr->next;
                }
                piece_moves = piece_moves->next;
        }

        return maxGame;
}

// return init_possible_moves[best] (depth 1)
struct gamestate *findBestMove(struct gamestate *curr) {
	struct MovesLists *init_allpossible_moves = getAllmoves(game, 1);
	struct LinkedList *piece_moves = init_allpossible_moves->next; //assuming NULL head
	//gamestate *bestMove = piece_moves->next;
	struct LinkedList *depth3scores = malloc(sizeof(struct LinkedList*));
        depth3scores->next = NULL;
        depth3scores->move = NULL;

	// calc scores & build depth3scores (find max for each depth 1 move)
	while (piece_moves != NULL) {
		struct gamestate *move = piece_moves->next;
		while (gameptr != NULL) {
			move->turn = 2; 
			
			//how to deal w/ C having no max function ***
			//see OPTION note below
			//move->score = max(findPossibleScores(move, move));

			findPossibleScores(move, move, depth3scores);	

			move = move->next;
		}
		piece_moves = piece_moves->next;
	}

	// traverse thru depth3scores & return its parent
	depth3scores = depth3scores->next; // has sentinel head
	struct gamestate *bestMove = depth3scores->next->move;
	while (depth3scores != NULL) {
		if (bestMove->score < depth3scores->move->score) {
			bestMove = depth3scores->move;
		}
		depth3scores = depth3scores->next;
	}
	
	return bestMove->parent;
}

// calculates scores for init_allpossible_moves (depth 3 moves)
// thinking 2 moves ahead
// parentGame is 1 of init_allpossible_moves
// this will return multiple results to append to LinkedList depth3scores
void findPossibleScores(struct gamestate *parentGame, struct gamestate *tmp, struct LinkedList *depth3scores) {
	
	// player's turn (this visited 1st)
	if (tmp->turn % 2 == 0) {
		struct MovesLists *allpossible_moves = getAllmoves(*tmp, 2);    
		struct gamestate *minGame = findMinGame(allpossible_moves);
		minGame->turn = 1;

		findPossibleScores(parentGame, minGame);
	}

	// AI's turn & we've reached game tree depth of 3
	else {
		struct MovesLists *allpossible_moves = getAllmoves(*tmp, 1);
		struct gamestate *maxGame = findMaxGame(allpossible_moves);
		maxGame->parent = parentGame;	
		/* APPEND THESE GAMES TO A LINKEDLIST & RETURN IT HERE
		*  TRAVERSE LINKEDLIST TO FIND MAX & RETURN ITS PARENT
		*  ITS PARENT IN findBestMove
		*/
		append(*depth3scores, *maxGame);

		return;
		//return maxGame->score;
	}

}

