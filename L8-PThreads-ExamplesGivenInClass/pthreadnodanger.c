#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define NUM_THREADS	4
int counter;
pthread_mutex_t mutexcounter; //extra piece of shared data

void* updateCounter(void* t){
	int i;
	long tid;
	tid = (long)t;
	printf("Thread %ld starting...\n",tid);
	pthread_mutex_lock (&mutexcounter); //before updating counter, you grab the lock
	for (i=0; i<100000; i++){ //each thread gonna run 100000 times 
		counter++;
	}
	pthread_mutex_unlock (&mutexcounter);
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
	
	pthread_mutex_init(&mutexcounter, NULL); //leaves it open, first thread to grab locks
	
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
	pthread_mutex_destroy(&mutexcounter);
	pthread_exit(NULL);
}