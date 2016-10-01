/*
Using primes.c as a template, write a program 
called moreprimes.c that prints the first 1000 primes. 
(The 1000th prime should be 7919).
*/
#include <stdio.h>
#include <math.h>

#define BOOL char
#define TRUE 1
#define FALSE 0

int moreprimes(int n){
	int count = 0; //to be incremented until n prime nums found
	int checkNum = 2; //number to check if prime
	BOOL primeFlag = TRUE; //assume prime until checked
	int i; //counter up to sqrt of number to check

	while(count < n){
		for(i=2; i<=sqrt(checkNum+0.0); i++){
			if(checkNum%i == 0){
				primeFlag = FALSE; //if checkNum is divisible by a number, it is not a prime number
				break; //take us out of for loop
			}
		}

		//at this point, primeFlag will tell us if 
		//we made it through the for loop without breaking
		if(primeFlag == TRUE){
			printf("%i\n", checkNum); //print the prime number
			count ++; //we have found one more prime number, so increment
		}
		checkNum ++; //check the next number while count < n
		primeFlag = TRUE; //primeFlag reset to true for the new number to check
	}

	return 0;
}

int main(void){
	moreprimes(1000); //print the first 1000 prime numbers
	return 0;
}
