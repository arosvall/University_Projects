#include <stdio.h>
 
void trim(char *str, char *dst) {
   int idx = 0;
   int pos = 0;
   
   int charFound = 0;
   char* st = str;
   while(st[idx] != '\0') {          //while there is more to check
      if(st[idx] == ' ') {        
          if(charFound == 0) {
           idx++;
          }
          else {
               if(st[idx+1] == ' ') { //if this is a space and next too, stop, no point continuing to iterate
                  break;
               } else {               //next char is a char, continue looking through string
                 dst[pos] = str[idx]; 
		 idx++;
 		 pos++;  }   
   	       }
          }
     else {   //if first occurence of a char, mark charFound to start string 
          if(charFound == 0){ 
              charFound = 1; }
        dst[pos] = st[idx];     //set the dst value as current str value
        idx++;                  //increase the position and index
        pos++;
       }
   }   	    


}

int main() {
    char s1[80];
    char s2[80];
    
    printf("Input string, max 80 characters \n");
    scanf(" %80[^\n]s", s1);

    
    trim(s1, s2);
    printf("Before: [%s]\n", s1);
    printf("After:  [%s]\n", s2);
 
}

