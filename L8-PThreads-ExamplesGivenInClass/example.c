#include <stdio.h>

int main(void){

	unsigned long voidlen1 = sizeof(void);
	unsigned long voidlen2 = sizeof(void*);
	unsigned long voidlen3 = sizeof(int);
	unsigned long voidlen4 = sizeof(int*);

	printf("%lu\t%lu\t%lu\t%lu\n",voidlen1,voidlen2,voidlen3,voidlen4);



}

void* someFunction(void* args){

   

}