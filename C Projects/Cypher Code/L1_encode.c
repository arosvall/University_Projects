#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* Program developed for NWEN243, Victoria University of Wellington
   Author: Kris Bubendorfer (c) 2015.
   Uses a modified caeser I & II

   Compile:  gcc -o encode encode.c

   // Make a file with a plain aplhabet in it:

   % cat > test
   ABCDEFGHIJKLMNOPQRSTUVWXYZ
   ^D

   // test it - this should be the correct result when encoding.

   % cat test | encode "i came, i saw"
   key: HJKLNOPQRTUVICAMESWXYZBDFG - 26
   HJKLNOPQRTUVICAMESWXYZBDFG

 */

char upcase(char ch){
  if(islower(ch))
    ch -= 'a' - 'A';
  return ch;
}

char* fixkey(char* s){
  int i, j;
  char plain[26]; // assume key < length of alphabet, local array on stack, will go away!

  for(i = 0, j = 0; i < strlen(s); i++){
    if(isalpha(s[i])){
      plain[j++] = upcase(s[i]);
    }
  }
  plain[j] = '\0'; 
  return strcpy(s, plain);
}

void buildtable (char* key, char* encode){
  int n = strlen(key) -1;
  int nk = strlen(key) -1;
  char temp[26];
 
  fixkey(key); // fix the key, i.e., uppercase and remove whitespace and punctuation

  char alph[26] = { 'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

  int k = sizeof(key);
  
   
   char* dup;
   char c; 
   int t = 0;
   for(int i = 0; i <= k; i++) {
     c = key[i];
     dup = strchr(temp, c);
       
     if(dup!= NULL) {}
     else { 
        encode[n] = c;
        temp[t] = c;
        n++;
	t++;
     }
}

int alphInd;
for(int i = 0; i < strlen(alph); i++) {
   if(alph[i] == c) {
       if(i==26) {
       c = 'A';
       alphInd = 0;
       break;
       }
       else {
       c = alph[i+1];
       alphInd = i+1;
       break;
      }
    }
 }
  
  while(n!=nk) {
  if(alphInd > 25) { alphInd = 0; }
  c = alph[alphInd];
  if(n > 25) { n = 0; }
  dup = strchr(temp, c);
  if(dup!= NULL) { }
  else { encode[n] = alph[alphInd]; temp[t] = alph[alphInd]; n++; t++; }
  alphInd ++;
}

}

int main(int argc, char **argv){
  // format will be: 'program' key {encode|decode}
  // We'll be using stdin and stdout for files to encode and decode.

  // first allocate some space for our translation table.

  char* encode = (char*)malloc(sizeof(char)*26);
  char ch;

  if(argc != 2){
    printf("format is: '%s' key", argv[0]);
    exit(1);
  }

  // Build translation tables, and ensure key is upcased and alpha chars only.

  buildtable(argv[1], encode);

  // write the key to stderr (so it doesn't break our pipes)

  fprintf(stderr,"key: %s - %d\n", encode, strlen(encode));

  // the following code does the translations.  Characters are read 
  // one-by-one from stdin, translated and written to stdout.

  ch = fgetc(stdin);
  while (!feof(stdin)) {
    if(isalpha(ch)){        // only encrypt alpha chars
      ch = upcase(ch);      // make it uppercase
      fputc(encode[ch-'A'], stdout);
    }else 
      fputc(ch, stdout);
    ch = fgetc(stdin);      // get next char from stdin
  }
}
  
