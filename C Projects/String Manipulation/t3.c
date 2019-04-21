#include <stdio.h>
#include <string.h>
void spacedel_ptr(char *t) {
    //no array indices
    int size = strlen(t);
    char temp[size];
    char *p = t;
    char *s = temp;
    while(*p != '\0') {
	if(*p != ' ') {
	s = p;
	*p++;
	*s++;
	}
    }

   strcpy(temp, t);
   t = temp;
	    

}
	
void spacedel_ary(char *t) { 
   //find the size of passed array without spaces and assign to a temp array      


   int size = 0;
   int arypos = 0;
   int arysize = 0;
   char *f;
   while(t[arypos] != '\0') {
       char c = t[arypos];
       if(t[arypos] != ' ') {
          size++;
       }
       arypos++;
   } 
    
   arysize = arypos+1;
   int pos = 0;
   char temp[size];
  
   //step through the passed array, adding non-spaces to a position in temp tracked by pos
    for(int i = 0; i <= arysize; i++) {
        char c = t[i];
        if(t[i] == ' ') { }
        else{
        temp[pos] = t[i];
        pos++;
	}
    }
  
   //assign temp to the passed array
   strcpy(t, temp); 
   t = temp;
}

int main() {
	
    
   
    printf("Enter another string of maximum 80 characters \n");
    char strq[81];
    scanf( " %80[^\n]s", strq);	

	printf("Original strq: [%s]\n", strq);
	spacedel_ary(strq);
	
	printf("Spaceless strq: [%s]\n", strq);
 


    printf("Enter a string of maximum 80 characters \n");
    char strp[81];
    scanf( " %80[^\n]s", strp);
    
  	printf("Original strp: [%s]\n", strp);
	spacedel_ptr(strp);
	printf("Spaceless strp: [%s]\n", strp);

}




