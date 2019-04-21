#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define ATTEMPTS 5
#define START 1
#define END 100

int get_secret(const int start, const int end) {
	srand(time(NULL));
	int randomNumber = (rand() % end + start);
	return randomNumber;
}

int get_guess(const int attempt) {
	printf("You have used %d attempts \n", attempt);
	int guess;
	scanf("%d", &guess);
	printf("Your guess was %d \n", guess);
	return guess;
}

int is_game_won(const int secret, const int guess){
	if(guess == secret) {
		printf("Your guess was correct! Well done! \n");
		return 1;
	}
	else if(guess < secret) {
		printf("My number is bigger than yours. \n");
		return 0;
	}	
	else {
	printf("My number is smaller than yours. \n");
	return 0;
	}
}

int main() {
char c;
c = 'y';
do {
	int winner = 0;
	int secret = get_secret(START, END);
	printf("Pick a number between 1 and 100 \n");
		for(int idx = 0; idx < ATTEMPTS; idx++) {
		int guess = get_guess(idx);
		int won = is_game_won(secret, guess);
			if(won == 1) {
			winner  = 1;
			break;
			}
		}	
	
	if(winner == 1) {
	printf("Well done you won! \n");}
	else {
	printf("My number was: %d \n", secret);
	}
	printf("Play again? (y/n) \n");
	scanf(" %c", &c);
	}	
while(c == 'y' || c == 'Y');
printf("Thanks for playing! \n");
}
