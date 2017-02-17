#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_THREADS	4 //try running with 1 or 2 threads
int counter;

void* updateCounter(void* t){
	int i;
	long tid;
	tid = (long)t;
	printf("Thread %ld starting...\n",tid); //each thread goes through this loop 100,000 times
	for (i=0; i<100000; i++){
		counter++; //BUT WE'RE SHARING THIS VARIABLE BTW THREADS
	}                                        
	printf("Thread %ld done.\n",tid);
	pthread_exit((void*) t);
}

int main (int argc, char* argv[]){
	pthread_t thread[NUM_THREADS];
	pthread_attr_t attr;
	int rc;
	long t;
	void *status;
	counter = 0;
	
	/* Initialize and set thread detached attribute */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(t=0; t<NUM_THREADS; t++) {
		printf("Main: creating thread %ld\n", t);
		rc = pthread_create(&thread[t], &attr, updateCounter, (void *)t); 
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	/* Free attribute and wait for the other threads */
	pthread_attr_destroy(&attr);
	for(t=0; t<NUM_THREADS; t++) {
		rc = pthread_join(thread[t], &status);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
		printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
	}
 	
	printf("counter = %d\n", counter);
	printf("Main: program completed. Exiting.\n");
	pthread_exit(NULL);
}