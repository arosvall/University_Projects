#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main () {
	int guess;
	srand(time(NULL));
	int randomNumber = (rand() % 100) +1;
	printf("Pick a number between 1 and 100 \n");
	scanf("%d", &guess);
	printf("Your number: %d \n", guess);
	if(guess == randomNumber) {
		 printf("Your guess was correct, well done! \n");
	}
	else if(guess < randomNumber) {
	   printf("My number was bigger than yours. \n");
	}
	else {
	printf("My number was smaller than yours. \n");
	}
	return 0;
}
