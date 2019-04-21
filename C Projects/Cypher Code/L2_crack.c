#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define TEXT_SIZE 200000  // Note, the longer the text the more likely you will get a good 'decode' from the start.
#define ALEN 26         // Number of chars in ENGLISH alphabet
#define CHFREQ "ETAONRISHDLFCMUGYPWBVKJXQZ" // Characters in order of appearance in English documents.
#define ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZ"

void charFreqCalc(char* coded, int* freq);
void charMap(char* decoded, int* freq);
/* Program developed for NWEN243, Victoria University of Wellington
   Author: Kris Bubendorfer, this extended version (c) 2015
   LAB: 2

   This program applies a basic frequency analysis on a cyphertext.  It has been extened over the 2014 version to
   solve polyalphabetic cyphers - by brute force.  In this case, it applies the frequency analysis for different 
   numbers of n keys (polyalphabetic Caeser).  Obviously it will need a cypher of about n times
   the typical length for a monoalphabetic cypher.

   Program is used like this:

   Compile:  gcc -o crack crack.c

   Test file (ctext): JWRLS, XSSH PZK JH HES BJFV, UZU (this is not a realistic length piece of cypher text)

   crack n

   Argument:

   n number of keys to try

   ---

   % cat ctext | crack 1
   ALICE, MEET YOU AT THE PARK, BOB   <-- of course it won't be this correct.  Don't worry about that for the -d option.
   AMFDE, UEET LNH AT TIE RASC, ONO   <-- this is what it really looks like, a larger sample is better, this is short.


 */


char upcase(char ch){
  if(islower(ch))
    ch -= 'a' - 'A';
  return ch;
}

int main(int argc, char **argv){
 // first allocate some space for our input text (we will read from stdin).

  char* text = (char*)malloc(sizeof(char)*TEXT_SIZE+1);
  char ch;
  int n, i;
 if(argc > 1 && (n = atoi(argv[1])) > 0); else{ fprintf(stderr,"Malformed argument, use: crack [n], n > 0\n"); exit(-1);} // get the command line argument n
  
  // Now read TEXT_SIZE or feof worth of characters (whichever is smaller) and convert to uppercase as we do it.
  // Added: changed to count frequencies as we read it in

  for(i = 0, ch = fgetc(stdin); i < TEXT_SIZE && !feof(stdin); i++, ch = fgetc(stdin)){
    text[i] = (ch = (isalpha(ch)?upcase(ch):ch));
  }
  text[i] = '\0'; // terminate the string properly.

  char* output = (char*)malloc(sizeof(char)*TEXT_SIZE+1);
  
 for(int num = 1; num <= n; num++) {
 	//open an array to store the letters
    char encoded[TEXT_SIZE/n];
	int idx = 0;
    
	//store the nth letter i.e. one key letter will increase by one, letter+=2 if 2 keys..., in the array
	for(int k = num-1; k < TEXT_SIZE; k+=n) {
	encoded[idx] = text[k];
	idx++;
	}
  //create arrays to store the frequency a letter occurs and the decode value	
  int freq[ALEN];
  char decoded[ALEN];
  //call other methods to do grunt work of calculating frequency and decode char
 charFreqCalc(encoded, freq);
 charMap(decoded, freq);
  //for each letter from original passed in text, increasing letter by n as above
  for(int k = num-1; k < TEXT_SIZE; k+=n) {
	  char c = text[k];
           
	  //step through the alphabet, find the letter that corresponds to letter from text
	  for(int i =0; i < ALEN; i++) {
	     char d = ALPHABET[i];
	     //if the letters match, look up the corresponding value in decode list and place in output array 
		 if(c == d) {
	        output[k] = decoded[i];
			break;
	     }
	  }
  }
 for(int i =0; i < strlen(output); i++) {
     char en  = output[i];
      fputc(en, stdout);
  }
}
}
/* Method to calculate the frequency a letter occurs to find the most likely fit from the alphabet */
void charFreqCalc (char* coded, int* freq) {
//for each letter of alphabet, 
for(int i = 0; i < ALEN; i++) {
char c = ALPHABET[i];
int f = 1;
   //count number of appearances in encoded text, store in the array
   for(int j = 0; j < strlen(coded); j++) {
   char d = coded[j];
     if(c == d) {
     f++;
     }
   }
freq[i] = f;
}
}
/*Method which looks up the CHFREQ table to find the most likely fit to a letter, the letter being counted in another method */
void charMap(char* decoded, int *freq) {
int alphInd = 0;
//find the max occurring letter
for(int i =0; i < ALEN; i++) {
  int max = 0; 
  int index;  
    for(int j = 0; j < ALEN; j++) {
	   if(freq[j] >= max) {
	   max = freq[j];
	   index = j;
	   }
	 }
//in this position (the position of most frequent letter), place the letter from the CHFREQ then move to next letter i.e. start with E, then T...	 
   decoded[index] = CHFREQ[alphInd];
   //set index to -1 to avoid repeats screwing with the table, increase to next letter in CHFREQ
   freq[index] = -1;   
   alphInd++;
 
 }
}
