#include <stdio.h>


void draw(const int size, char *field) {

printf("\n \n");
printf("+-+");

for(int idx =0; idx < size-1; idx++) {
	printf("-+");	
}
printf("\n");
printf("|");
for(int idx = 0; idx < size; idx++) {
	printf("%c|", field[idx]);
}

printf("\n");
printf("+-+");
for(int idx = 0; idx < size-1; idx++) {
	printf("-+");
}
printf("\n");
for(int idx = 0; idx < size; idx++) {

	printf("%2d", idx+1);
}
printf("\n");
}


int add_cross(const int size, char *field, const int position) {
	if(position >= size) {
	printf("Index is out of range! \n");
	return -1;
	}	
	if(field[position] == 'X') {
	printf("Position is already taken! \n");
	return 0;
	}

	else if(field[position] != 'X') {
	field[position] = 'X';
	return 1;
	}
}

int is_solved(const int size, char *field) {
	int count = 0;
	for(int i =0; i < size; i++) {
	if(field[i] == 'X') {
	count++;
	} else {
	count = 0;
	}
	if (count == 3) {
	return 1;
	}
}
return 0;
}



int main() {

int fieldSize;
printf("Enter the size of the field \n");
scanf("%d", &fieldSize);

char field[fieldSize];
	for(int i =0; i < fieldSize; i++) {
	field[i] = ' ';
	}

int solved = 0;
char player = 'A';
while(solved != 1) {
	
	draw(fieldSize, field);
	printf("Pick a space player %c \n", player);
	int position;
	scanf("%d", &position);
	printf("Player %c : %d \n", player, position);
	position = position-1;	
	add_cross(fieldSize, field, position);
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
