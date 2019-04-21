#include <stdlib.h>
#include "k.h"
#include <stdio.h>

void add_random_tile(struct game *game){
	int row, col;
	
	// find random, but empty tile
	do{
		row = rand() % 4;
		col = rand() % 4;
	}while(game->board[row][col] != ' ');

	// place to the random position 'A' or 'B' tile
	int tile = 'A' + (rand() % 2);
	game->board[row][col] = tile;
}

//draw the game board, the same style as Assignment 2
void render(const struct game game) {
  printf("\n \n");
  printf("Score is: %d \n", game.score);
  printf("+"); 
    for(int i = 0; i < 4; i ++) {
       printf("--+");
    }
 
    for(int i = 0; i < 4; i++) {
        printf("\n"); 
        printf("|");
       
        for(int j = 0; j < 4; j++) {
            printf(" %c|", game.board[i][j]);
        }
    
        printf("\n");
        printf("+");
        for(int idx = 0; idx <4; idx++) {
             printf("--+");
        }
    }
  printf("\n"); 
}



//method to check if game is won
bool is_game_won(const struct game game) {
    //step along each element of the 4x4 grid, returning true if k is found or false if it is not

    for(int row = 0; row < 4; row++) {
       for(int col = 0; col < 4; col++) {
          if(game.board[row][col] == 'K') {
	    return true;
           }
        }
     }

  return false;
}


bool is_move_possible(const struct game game) {
  char current;
      for(int row = 0; row < 4; row ++) {
        for(int col = 0; col < 4; col++) {
           current = game.board[row][col];
	
           //if there is an empty space, return true a move is possible
           if(current == ' ') { 
              return true; 
	   }

          //check if any surrounding squares contain the same character, if so return true
	  //check the up and down, then left and right (Split into two checks for clarity) 
           else if(game.board[row+1][col] == current || game.board[row-1][col] == current) {
	      return true; 
 	   }
	   
           else if(game.board[row][col+1] == current || game.board[row][col-1] == current) {
	      return true;
           }
       }
     }
    //if there are no avaliable moves by the above criteria, return false
    return false;
}


bool update(struct game *game, int dy, int dx) {
   //check the movements are valid
    if(dy == 0 && dx == 0) { return false; }
    if(dy != 0 && dx != 0) { return false; }
    
    //break the search method into functions to avoid endless text in this function
    if(dy == 1)  return up(game);
    if(dy == -1) return down(game);
    if(dx == 1)  return right(game);
    if(dx == -1) return left(game);   

   return false;
}

//check the movement in the up direction, with it's following code
bool up(struct game *game)  {
    char current;
    char above; 
    bool stateChanged = false;
    for(int col = 0; col < 4; col++) {
    int row = 3;
    //initialise a boolean array to track movement
    bool status[4];
    for(int i = 0; i < 4; i++) { status[i] = false; }

    //move white space to the bottom of the col
    while(row >= 0) { 
         current  = game->board[row][col];
         bool backwards = false;
         //if the current space is not empty, check if there is an empty above. if yes, swap values and return to the bottom of the col and try again
         if(current != ' ') {
            above = game->board[row-1][col];
              if(above == ' ') {
                game->board[row-1][col] = current;
                game->board[row][col] = ' ';
                backwards = true;
                stateChanged = true;
                //move the boolean status with it's char
                status[row-1] = true;
                status[row] = false;
                }
        //if tile above current tile is the same, calculate the new tile and assign that to above square, replacing this with an empty space and checking through for empty spaces again
              else if(current == above && status[row-1] == false) {
                game->board[row-1][col] = newChar(current, game);
                game->board[row][col] = ' ';
                backwards = true;
                stateChanged = true;
                status[row-1] = true;
              }
            }
          
         if(!backwards){ 
            row--;
         }
         else { 
            row = 3;
         }   
                 
    }
  }

     return stateChanged;

}

bool down(struct game *game)  {
    char current;
    char below; 
    bool stateChanged = false;
    for(int col = 0; col < 4; col++) {

    //initialise a boolean array to track movement
    bool status[4];
    for(int i = 0; i < 4; i++) { status[i] = false; }

    int row = 0;
      //move white space to the top of the col
    while(row < 4) { 
         current  = game->board[row][col];
         bool backwards = false;
         //if the current space is not empty, check if there is an empty space below. if yes, swap values and return to the top of the col and try again
         if(current != ' ') {
            below = game->board[row+1][col];
              if(below == ' ') {
                game->board[row+1][col] = current;
                game->board[row][col] = ' ';
                backwards = true;
                stateChanged = true;
                status[row+1] = true;
                status[row] = false; 
                }
        //if tile above current tile is the same, calculate the new tile and assign that to above square, replacing this with an empty space and checking through for empty spaces again
              else if(current == below && status[row+1] == false) {
                game->board[row+1][col] = newChar(current, game);
                game->board[row][col] = ' ';
                backwards = true;
                stateChanged = true;
                status[row+1] = true;
              }
            }
         
 
         if(!backwards){ 
            row++;
         }
         else { 
            row = 0;
         }   
                 
    }
  }

     return stateChanged;

}


bool right(struct game *game)  {
    char current;
    char next; 
    bool stateChanged = false;
    for(int row = 0; row < 4; row++) {
    int col = 0;
    //initialise a boolean array to track movement
    bool status[4];
    for(int i = 0; i < 4; i++) { status[i] = false; }
   
   //move white space to the right of the row
    while(col < 3) { 
         current  = game->board[row][col];
         bool backwards = false;
         //if the current space is not empty, check if there is an empty space below. if yes, swap values and return to the top of the col and try again
         if(current != ' ') {
            next = game->board[row][col+1];
              if(next == ' ') {
                game->board[row][col+1] = current;
                game->board[row][col] = ' ';
                backwards = true;
                stateChanged = true;
                status[col+1] = true;
                status[col] = false; 
               }
        //if tile above current tile is the same, calculate the new tile and assign that to above square, replacing this with an empty space and checking through for empty spaces again. status check is to stop double moves
              else if(current == next && status[col+1] == false) {
                game->board[row][col+1] = newChar(current, game);
                game->board[row][col] = ' ';
                backwards = true;
                stateChanged = true;
                status[col+1] = true;
              }
            }
         
 
         if(!backwards){ 
            col++;
         }
         else { 
            col = 0;
         }   
                 
    }
  }

     return stateChanged;

}
bool left(struct game *game)  {
    char current;
    char left; 
    bool stateChanged = false;
    for(int row = 0; row < 4; row++) {
    int col = 3;
   
    //initialise a boolean array to track movement
    bool status[4];
    for(int i = 0; i < 4; i++) { status[i] = false; }
      //move white space to the left of the row
   
    while(col >= 1) { 
         current  = game->board[row][col];
         bool backwards = false;
         //if the current space is not empty, check if there is an empty to the left. if yes, swap values and return to the end of the col and try again
         if(current != ' ') {
            left = game->board[row][col-1];
              if(left == ' ') {
                game->board[row][col-1] = current;
                game->board[row][col] = ' ';
                backwards = true;
                stateChanged = true;
                status[col-1] = true;
                status[col] =false;
                }
        //if tile left of current tile is the same, calculate the new tile and assign that to previous square, replacing this with an empty space and checking through for empty spaces again
              else if(current == left && status[col-1] ==false) {
                game->board[row][col-1] = newChar(current, game);
                game->board[row][col] = ' ';
                backwards = true;
                stateChanged = true;
                status[col-1] = true;
              }
            }
        
 
         if(!backwards){ 
            col--;
         }
         else { 
            col = 3;
         } 
     }
   }
     return stateChanged;
}



/* Depending on char, increase the score and return the next letter in the alphabet */

char newChar(char a, struct game *game) {
char new;
if(a == 'A') { 
   game->score+=4;
   new = 'B';
}
else if(a == 'B') {
   game->score+=8;
   new = 'C';
}
else if(a == 'C') {
   game->score+=16;
   new = 'D';
}
else if(a == 'D') {
   game->score += 32;
   new = 'E';
}
else if(a == 'E') {
   game->score += 64;
   new = 'F';
}
else if(a == 'F') {
   game->score += 128;
   new = 'G';
}
else if(a == 'G') {
   game->score +=256;
   new = 'H';
}
else if(a == 'H') {
   game->score += 516;
   new = 'I';
}
else if(a == 'I') {
   game->score += 1024;
   new = 'J';
}
else {
   new = 'K';
}
return new;
}


