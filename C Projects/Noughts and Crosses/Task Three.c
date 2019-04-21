#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void draw(const int size, char field[][size]) {

printf("\n \n");

printf("   +-+");
for(int idx = 0; idx < size-1; idx++) {
	printf("-+");
	}

for(int i = size-1; i >= 0; i--) {	
	printf("\n");
	printf("%d", i+1);	

	printf("  |");
		for(int idx = 0; idx < size; idx++) {
		printf("%c|", field[i][idx]);
		}
	
	printf("\n");
	printf("   +-+");
		for(int idx = 0; idx < size-1; idx++) {
		printf("-+");
		}
	}
	
	printf("\n");
	printf("   ");
		for(int idx = 0; idx < size; idx++) {
		printf("%2d", idx+1);
		}
	
	printf("\n");
	}



int add_cross(const int size, char field[][size], const int x, const int y) {

	if(x >= size || y >= size) {
	printf("Index is out of range! \n");
	return -1;
	}	

	if(field[x][y]!= ' ') {
	printf("Position is already taken! \n");
	return 0;
	}

	else {
	field[x][y] = 'X';
	}

	return 1;

}

int is_solved(const int size, char field[][size]) {
for(int i = 0; i < size; i++) {
for(int j = 0; j < size; j++) {

if(field[i][j] != ' ') {
char c = field[i][j];


if(field[i+1][j] == c) {
   if(field[i-1][j] == c || field[i+2][j] == c) { return 1; } }

if(field[i-1][j] == c) {
   if(field[i-2][j] == c) { return 1; } }

if(field[i][j+1] == c) {
   if(field[i][j-1] == c || field[i][j+2] == c) { return 1; } }	

if(field[i][j-1] == c) {
   if(field[i][j-2] == c) { return 1; } }

if(field[i+1][j+1] == c) {
   if(field[i-1][j-1] == c || field[i+2][j+2] == c) { return 1; } }

if(field[i-1][j-1] == c) {
   if(field[i-2][j-2] == c) { return 1; } }

if(field[i-1][j+1] == c) {
   if(field[i-2][j+2] == c || field[i+1][j-1] == c) { return 1; } }

if(field[i+1][j-1] == c) {
   if(field[i+2][j-2] == c) { return 1; } }

}       //closing check to see if the ijth entry is not ' '
}   	//closing for loop j
} 	//closing for loop i


return 0;
}


void make_turn(const int size, char field[][size]) {
int completed = -1;
while(completed != 0) {

srand(time(NULL));
int x = (rand() % size);
int y = (rand() % size);

if(field[x][y] == ' ')  {
   field[x][y] = 'O';
   completed = 0;
}
if(completed == 0) { printf("Computer: %d %d \n", x, y);
}
}
}

int main() {

int fieldSize;
printf("Enter the size of the field \n");
scanf("%d", &fieldSize);
if(fieldSize < 3 || fieldSize > 10) {
printf("Invalid grid size, please choose a number between 3 and 10 \n");
scanf("%d", &fieldSize);
}

char field[fieldSize][fieldSize];


	for(int i =0; i < fieldSize; i++) {
	for(int j =0; j < fieldSize; j++) {	
		field[i][j] = ' ';
	}
	}



int solved = 0;
char player = 'A';
while(solved != 1) {
	
	draw(fieldSize, field);
	if(player == 'A') {
	printf("Pick a row and column position\n");
	
	int x, y;
	scanf("%d %d", &x, &y);
	printf("Player %c : %d %d \n", player, x, y);
	x = x-1;	
	y = y-1;

	
	add_cross(fieldSize, field, x, y);
	} 
	else {
	make_turn(fieldSize, field);
	}
	solved = is_solved(fieldSize, field);	
		if(solved == 1) {
		draw(fieldSize, field);
		if(player == 'A') {
		printf("Congratulations Player, you won!!\n");
		} else {
		printf("Computer won, better luck next time! \n");
		}
		}
	
	if(player == 'A') {
	player = 'B';
	}
	else {
	player = 'A';
	}

	}
return 0;
}
