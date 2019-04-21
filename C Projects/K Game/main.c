#include "k.h"
#include <stdio.h>
int main() {
   //initialise the game and a variable to track completion
   struct game kGame; 

   for(int i =0; i < 4; i++) {
      for(int j =0; j <4; j++) {
          kGame.board[i][j] = ' ';
      }
   }
   kGame.score = 0;
   int completed = 0;
   add_random_tile(&kGame);

   while(completed == 0) {
   render(kGame);
   //user input to decide direction board moves in
   printf("To terminate game, press 5 \n");
   printf("Pick a direction using the wasd keys \n");
   char dir;
   scanf(" %c", &dir);
   //if dir isn't a chosen variable
   while(dir!= 'w' && dir!= 'a' && dir!= 's' && dir!='d' && dir!='5') {
   printf("Incorrect input, pick a direction using wasd");
   scanf(" %c", &dir);
   }
   //depending on direction of movement, update the game board and add a tile
   if(dir == 'w')      { update(&kGame, 1, 0); }
   else if(dir == 'a') { update(&kGame, 0, -1); }
   else if(dir == 's') { update(&kGame, -1, 0);  }
   else if(dir == 'd') { update(&kGame, 0, 1);  }
   else if(dir == '5') { completed = 3; break; }
   add_random_tile(&kGame);

   //check if the game is won, or if there are still moves left
   if(is_game_won(kGame)) { 
      render(kGame);   
      completed = 1; }
   
   if(!is_move_possible(kGame)) { 
      render(kGame);
      completed = 2; } 
   }

   //once game is won or end has been reached
   if(completed == 1) { printf("Congratulations, you won! \n"); }
   else if(completed ==3) { printf("Game terminated \n"); }
   else { printf("No more moves, better luck next time!"); }

return 0;
}

