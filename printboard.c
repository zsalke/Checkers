#include <stdio.h>
#include <stdlib.h>

#define EMPTY 0
#define CHECK 1
#define KING 2

void printboard(int gamestate[]){
	int count = 0;
	//for (int i = 0; i < 8; i++) {
	//	printf("__");
	//}
	printf("\n");
	for (int i = 0; i <= 16; i++){
		printf("|");
		printf(" ");
		if (gamestate[count] == CHECK){
			printf("c");
		} else if (gamestate[count] == KING) {
			printf("C");
		} 
		printf(" ");
		count++;
		if (count%8 == 0) {
		printf("|\n");
		
		}
	}
	printf("\n");
}

int main(){
	int arr[16] = {1, 0, 1, 0, 2, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1};
	printboard(arr);
}
