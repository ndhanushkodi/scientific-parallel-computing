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
	BOOL trackPrimes[n+1];
	int c;
	for(c=0; c<n+1; c++){
		trackPrimes[c] = TRUE;
	}

	int i;
	int j;
	double sqOfN = sqrt(n+0.0);
	int sqOfNInt = (int)floor(sqOfN);


	for(i=2; i<=sqOfNInt+1; i++) {
		if (trackPrimes[i] == TRUE){
			for(j=i*i; j<=n; j+=i){
				trackPrimes[j] = FALSE;
			}
		}
	}

	int k;
	for(k=2; k<n+1; k++){
		if(trackPrimes[k] == TRUE){
			printf("%i\n", k);
		}
	}
	return 0;

}

int main(void) {
	primes(100);

	return 0;
}