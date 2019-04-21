#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define ATTEMPTS 5
#define START 1
#define END 100


int main() {
	char c;
	c = 'y';
	do {
	int found = 0;
	srand(time(NULL));
	int randomNumber = (rand() % END) + START;
	printf("Pick a number between 1 and 100, you have at most 5 tries. \n");	for(int idx = 0; idx < ATTEMPTS; idx++) {
	int guess;
	scanf("%d", &guess);
		printf("Your guess: %d \n", guess);
		if(guess == randomNumber) {
			printf("Your guess of was correct! Well done!");
			found = 1;
			break;		
		}	
		else if(randomNumber > guess) {
			printf("My number is bigger than yours. \n");
		}
		else if(randomNumber < guess) {
			printf("My number is smaller than yours. \n");
		}		
	}	
	if(found ==0) {
		printf("Game over. My number was: %d . \n", randomNumber);
	}
	printf("Play again? (y/n)");
	scanf(" %c", &c);

	}
	while(c == 'y' || c == 'Y');
	printf("See you later! \n" );
	return 0;
}
