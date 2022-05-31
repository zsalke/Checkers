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
		if (game->player_pieces[i]) {
			num_playerpieces++;
			if (game->player_pieces[i]->value == KING) {
				num_playerpieces++; // extra pt for kings
			}
		}
		if (game->ai_pieces[i]) {
                        num_aipieces++;
			if (game->ai_pieces[i]->value == XKING) {
                                num_aipieces++; // extra pt for kings
                        }
                }
	}

	return num_aipieces - num_playerpieces;
}

// finds board w/ min score to help AI get best move in reponse to player's best
// MovesLists are nodes themselves
struct gamestate *findMinGame(struct MovesLists *allpossible_moves) {
	
	if (!allpossible_moves->next) {
		mvprintw(0, 0, "No possible moves\n");
		endwin();
		exit(2);
	}

	struct gamestate *minGame;

	// Set min game to first possible gamestate
	struct MovesLists *buffer = allpossible_moves;
	while (buffer->next) {
		buffer = buffer->next;
		if (buffer->list->next) {
			minGame = buffer->list->next->move;
		}
	}



	while (allpossible_moves->next) {

		allpossible_moves = allpossible_moves->next;
		struct LinkedList *piece_moves;
		if (allpossible_moves->list) {
			piece_moves = allpossible_moves->list;
		}
		
		while (piece_moves->next) {
			piece_moves = piece_moves->next;

			struct gamestate *gameptr = piece_moves->move;

			//NEED A UPDATE GAMESTATE FUNCTION ***
			//-wait checkmoves.c returns an updated board
			//(update ai_pieces, player_pieces, board, captures etc)
			
			gameptr->score = calc_score(gameptr);
			
			if (gameptr->score < minGame->score) {
				minGame = gameptr; //dk if work cuz &'s			
			}
		}
	}
	
	return minGame;
}

struct gamestate *findMaxGame(struct MovesLists *allpossible_moves) {
	
	if (!allpossible_moves->next) {
		mvprintw(0, 0, "%s\n", "No possible moves\n");
		endwin();
		exit(2);
	}

	struct gamestate *maxGame;

	// Set max game to first possible gamestate
	struct MovesLists *buffer = allpossible_moves;
	while (buffer->next) {
		buffer = buffer->next;
		if (buffer->list->next) {
			maxGame = buffer->list->next->move;
		}
	}

	while (allpossible_moves->next) {

                allpossible_moves = allpossible_moves->next;

                struct LinkedList *piece_moves = allpossible_moves->list;

                while (piece_moves->next) {
                        piece_moves = piece_moves->next;

                        struct gamestate *gameptr = piece_moves->move;

                        //NEED A UPDATE GAMESTATE FUNCTION ***
                        //-wait checkmoves.c returns an updated board
                        //(update ai_pieces, player_pieces, board, captures etc)

                        gameptr->score = calc_score(gameptr);

                        if (gameptr->score < maxGame->score) {
                                maxGame = gameptr; //dk if work cuz &'s                 
                        }
                }
        }

        return maxGame;
}




// calculates scores for init_allpossible_moves (depth 3 moves)
// thinking 2 moves ahead
// parentGame is 1 of init_allpossible_moves
// this will return multiple results to append to LinkedList depth3scores
void findPossibleScores(struct gamestate *parentGame, struct gamestate *tmp, struct LinkedList *depth3scores) {

        // player's turn (this visited 1st)
        if (tmp->turn % 2 == 0) {
                struct MovesLists *allpossible_moves = getAllmoves(tmp, 1);
	        struct gamestate *minGame = findMinGame(allpossible_moves);

                minGame->turn = 1;

                findPossibleScores(parentGame, minGame, depth3scores);
        }

        // AI's turn & we've reached game tree depth of 3
        else {
                struct MovesLists *allpossible_moves = getAllmoves(tmp, 0);
                struct gamestate *maxGame = findMaxGame(allpossible_moves);
//		mvprintw(0, 0, "Address of maxGame: %p\n", maxGame);
                maxGame->parent = parentGame;
                /* NOTE: APPEND THESE GAMES TO A LINKEDLIST & RETURN IT HERE
                *  TRAVERSE LINKEDLIST TO FIND MAX & RETURN ITS PARENT
                *  ITS PARENT IN findBestMove
                */
                append(depth3scores, maxGame);

                return;
                //return maxGame->score;
        }
}


// return init_possible_moves[best] (depth 1)
struct gamestate *findBestMove(struct gamestate *curr) {
	struct MovesLists *init_allpossible_moves = getAllmoves(curr, 0);
	struct LinkedList *depth3scores = malloc(sizeof(struct LinkedList*));
        depth3scores->next = NULL;
        depth3scores->move = NULL;
	
	// calc scores & build depth3scores (find max for each depth 1 move)
	while (init_allpossible_moves->next) {
		
		init_allpossible_moves = init_allpossible_moves->next;
		struct LinkedList *piece_moves = init_allpossible_moves->list;
		
		while (piece_moves->next) {

			piece_moves = piece_moves->next;
			struct gamestate *move = piece_moves->move;
			move->turn = 2; 
			
			//how to deal w/ C having no max function ***
			//see NOTE below
			//move->score = max(findPossibleScores(move, move));

			findPossibleScores(move, move, depth3scores);	

		}
	}

	// traverse thru depth3scores & return its parent
	struct gamestate *bestMove = depth3scores->next->move;
	while (depth3scores->next) {
		depth3scores = depth3scores->next;
		if (bestMove->score < depth3scores->move->score) {
			bestMove = depth3scores->move;
		}
	}
	
	return bestMove->parent;
}
