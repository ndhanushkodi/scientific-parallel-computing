#include <stdio.h>
#include <math.h>
/*
Write a program called primes.c that 
will print to the screen all of the prime 
numbers between 1 and 1000, inclusive.
*/
#define BOOL char
#define FALSE 0
#define TRUE 1

/*
Reference https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
*/
int primes(int n){
	BOOL trackPrimes[n+1]; //n+1 so the indeces will be 0-n inclusive
	int c;
	for(c=0; c<n+1; c++){
		trackPrimes[c] = TRUE; //initialize the trackPrimes array as true
		//the value of the array at some index (true/false)
		//indicates whether the index is prime
	}

	int i;
	int j;
	double sqOfN = sqrt(n+0.0);
	int sqOfNInt = (int)floor(sqOfN); //made an int version of sqOfN, and rounded up in the loop

	for(i=2; i<=sqOfNInt+1; i++) { //for all i's up to the sqrt of n
		if (trackPrimes[i] == TRUE){
			for(j=i*i; j<=n; j+=i){ 
			//cross out multiples of i starting at i^2 
			//because we have already checked multiples 
			//of i before i^2
				trackPrimes[j] = FALSE; //this ensures we cross out all non prime indeces
			}
		}
	}

	int k;
	for(k=2; k<n+1; k++){
		if(trackPrimes[k] == TRUE){
			printf("%i\n", k); //print the indeces that are prime
		}
	}
	return 0; //no errors, could have had a void method instead if I wanted

}

int main(void) { //runs the program
	primes(1000); //print prime numbers up to 1000
	return 0;
}