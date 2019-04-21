#include <stdio.h>

int swap_ptr(int** ptr1, int** ptr2) {
   int temp = **ptr1;
   **ptr1 = **ptr2;
   **ptr2 = temp;
   
   return 0;
}

// TODO – here you implement your swap function  

int main(void)

{
	int p = 11, q = 22; 
	int *ptrp = &p, *ptrq = &q;
	int **ppp = &ptrp, **ppq = &ptrq;

	printf("Before: %d %d", p, q);

	swap_ptr(ppp,ppq); /* &ptrp, &ptrq passed */

	printf("After:  %d %d", p, q);
}

