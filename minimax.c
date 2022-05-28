#include <stdio.h>
#include <string.h>
#include "board.h"
#include "checkmove.h"

// move this to main or something
void changeTurn(gamestruct game) {
	if (game->turn % 2 == 0) { // player made a move
		game->turn = 1; // now AI's turn
	} else {
		game->turn = 2; //dk if we're incrementing or just assigning
	}
}
// 

int calc_score(gamestate *game) {
	int num_playerpieces = 0;
	int num_aipieces = 0;
	for (int i = 0; i < 12; i++) {
		if (game->player_pieces[i] != NULL) {
			num_playerpieces++;
		}
		if (game->ai_pieces[i] != NULL) {
                        num_aipieces++;
                }
	}

	return num_aipieces - num_playerpieces;
}

// finds board w/ min score to help AI get best move in reponse to player's best
// MovesLists are nodes themselves
gamestate findMinGame(MovesLists *allpossible_moves) {
	LinkedList *piece_moves = allpossible_moves->list;
	gamestate *minGame = piece_moves->next->move;

	while (piece_moves != NULL) {
		gamestate *gameptr = piece_moves->move;
		while (gameptr != NULL) {
			
			//NEED A UPDATE GAMESTATE FUNCTION ***
			//(update ai_pieces, player_pieces, board, captures etc)
			
			gameptr->score = calc_score(gameptr);
			
			if (gameptr->score < mingame->score) {
				minGame = gameptr; //dk if work cuz &'s			
			}
			gameptr = gameptr->next;
		}
		piece_moves = piece_moves->next;
	}

	return minGame;
}

// return init_possible_moves[best] (depth 1)
gamestate findBestMove(gamestate *curr) {
	MovesLists init_allpossible_moves = getAllmoves(game, 1);
	LinkedList piece_moves = init_allpossible_moves->next; //assuming NULL head
	gamestate bestMove = piece_moves->next;

	// calc scores
	while (piece_moves != NULL) {
		gamestate move = piece_moves->next;
		while (gameptr != NULL) {
			move->turn = 2; 
			
			//how to deal w/ C having no max function ***
			//see OPTION note below
			move->score = max(findPossibleScores(move, move));

			if (move->score > bestMove->score) {
				bestMove = move; //&'s may conflict
			}
			move = move->next;
		}
		piece_moves = piece_moves->next;
	}
	
	return bestMove;
}

// calculates scores for init_allpossible_moves (depth 1 moves)
// thinking 2 moves ahead
// (technically the scores of 2 moves ahead)
// parentGame is 1 of init_allpossible_moves
void findPossibleScores(gamestate *parentGame, gamestate *tmp) {
	
	// player's turn (this visited 1st)
	if (tmp % 2 == 0) {
		MovesLists *allpossible_moves = getAllmoves(tmp, 2);    
		// player's best move possible
		gamestate minGame = findMinGame(allpossible_moves);
		//parentGame->score = minGame->score;
		minGame->turn = 1;
		//minGame->parent = parentGame;

		//get rid of parent stuff
		findPossibleScores(parentGame, minGame);
	}
	// AI's turn & we've reached game tree depth of 3
	else {
		MovesLists *allpossible_moves = getAllmoves(tmp, 1);
		//LinkedList piece_moves = allpossible_moves->list;
		//gamestate gameptr = piece_moves->next->move; //yo imlost doublecheck
	
		//do similar as above except maxGame
		// find maxGame in this tree branch & return its int score
		
		/* OPTION: APPEND THESE GAMES TO A LINKEDLIST & RETURN IT HERE
		*  TRAVERSE LINKEDLIST TO FIND MAX & RETURN ITS PARENT
		*  ITS PARENT IN findBestMove
		/

		//return maxGame->score;
	}

}
