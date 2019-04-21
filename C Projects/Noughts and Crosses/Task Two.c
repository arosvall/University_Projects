#include <stdio.h>


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



int add_cross(const int size, char field[][size], const int x, const int y, const char player) {

	if(x >= size || y >= size) {
	printf("Index is out of range! \n");
	return -1;
	}	

	if(field[x][y]!= ' ') {
	printf("Position is already taken! \n");
	return 0;
	}

	else if(player == 'A') {
	field[x][y] = 'X';
	}
	else {
	field[x][y] = 'O'; 
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
	printf("Pick a row and column position Player %c \n", player);
	
	int x, y;
	scanf("%d %d", &x, &y);
	printf("Player %c : %d %d \n", player, x, y);
	x = x-1;	
	y = y-1;

	
	add_cross(fieldSize, field, x, y, player);
	solved = is_solved(fieldSize, field);	
		if(solved == 1) {
		draw(fieldSize, field);
		printf("Congratulations Player %c, you won!!\n", player);
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
