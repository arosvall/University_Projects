#include <stdio.h>
#include <string.h>


char* my_strchr(char* str, char c)
{
    //find the character if it exists and create a new array to return
    int strSize = strlen(str);
    int pos = 0;
    int size = 0;
    int found = 0;
    int idx = 0;
    //look through array, find the character, create a new array with the values following from that character and return that. Otherwise, return null as the char is not in the string

    while(str[idx] != '\0') {
       if(str[idx] == c) { 
          pos = idx;
	  size = strSize-pos;
          char sub[size]; 		//returning array made from strsize-pos
             while(str[idx]!= '\0') {
		sub[pos-size] = str[idx];
		printf("Char: %c \n", sub[pos-size]);
		idx++;
		pos++;
	      }
	 strcpy(str,sub);
	 return str;	 
          }
	else {
	idx++;
	}
    }
    return NULL;

}


int main()
{
	printf("Enter the string: ");
	char str[81];
        scanf(" %80[^\n]s", str);
       
	

	printf("Enter the character: ");
        char c;
	scanf(" %c", &c);
	

	char* result = my_strchr(str, c); 

	if (result == NULL)
	{
		printf("Couldn't find %c.\n", c);
	}
	else
	{
		printf("Substring: %s \n", result);
	}
}
