/*
 * Sidd Singal & Nitya Dhanushkodi
 * Scientific & Parallel Computing
 * Problem Set 6
 * palindromicparallel
 *
 * Given a dictionary of line seperated words, this program finds the number
 * of palindromic pairs in that dictionary. Two words are a palindromic pair
 * if reversing one yields the other (i.e. bats and stab). Note that this program
 * is case sensitive, so Bats and stab are not palindromic.
 */

// external libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// dictionary specific constants. Make sure to change these
// if changing the dictionary
#define NUM_WORDS 235886
#define MAX_WORD_LEN 30

// boolean values
#define false 0
#define true 1

// shared data
char* words[NUM_WORDS];
int indices[27];
int wordIndex;
int totPalWords;
int* wordsFoundByThread;

// mutexes for "dangerous" data
pthread_mutex_t indexMutex;

// functions are defined below the main method
void reverseWord(char* reversedWord, char* originalWord);
char getFirstCharLower(char* word);
int doesWordExist(int startI, int endI, char* word);
void* findPalindromicsAtLetter(void* t);

/*
 * Finds the number of palindromic words in a dictionary of words
 *
 * inputs
 *		numWorkers: the number of threads to run this program
 */
int main(int argc, char* argv[]){

	// inits
	int i, j, len;
	int numWorkers;
	totPalWords = 0;
	wordIndex = 0;
	char firstL;

	// Before we do anyhting, make sure we have enough arguments.
	// There should be at least one argument which is the number
	// of workers to run this program with
	if(argc < 2){
		printf("You must supply at least one argument, which is the number of workers. Exiting Program.\n");
		return -1;
	}

	// We want to store the amount of word
	// each worker finds
	numWorkers = atoi(argv[1]);
	wordsFoundByThread = malloc(sizeof(int)*numWorkers);
	for(i = 0; i < numWorkers; i++){
		wordsFoundByThread[i]=0;
	}

	// pthread inits
	pthread_t thread[numWorkers];
	pthread_attr_t attr;
	int rc;
	long t;
	void *status;



	//location is system specific - this works for mac 
	FILE* fp = fopen("wordsdict.txt", "r");
	if(!fp){
		printf("Unable to access dictionary\n");
		return -1;
	}

	// We are going to mark all the indices where we start
	// to look at a different first letter in a dictionary.
	// This will make it easy to give different tasks
	// to different workers based on starting letter.
	for(i = 0; i < 27; i++){
		indices[i] = -1;
	}

	//Read file into array of strings and set up indices
	for(i = 0; i < NUM_WORDS; i++){

		// Read and store the word
		words[i] = malloc(sizeof(char) * MAX_WORD_LEN);
		fgets(words[i], MAX_WORD_LEN, fp);
		len = strlen(words[i]);
		words[i][len-1] = '\0';

		// If we haven't stored the starting index for some character
		// then store it once we get to it
		firstL = getFirstCharLower(words[i]);
		if(indices[firstL-'a']==-1){
			indices[firstL-'a']=i;
		}
	}

	// Set the index for the end of the list
	indices[26]=i;

	// Fill in any values in indices that hasn't already
	// been filled out
	for(i = 25; i > 0; i--){
		if(indices[i]==-1){
			indices[i]=indices[i+1];
		}
	}

	// Close the file
	fclose(fp);

	// initialize mutex for the index variable
	pthread_mutex_init(&indexMutex, NULL);

	// Initialize and set thread detached attribute
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// start the process for each thread
	for(t=0; t<numWorkers; t++) {
		printf("Main: creating thread %ld\n", t);
		rc = pthread_create(&thread[t], &attr, findPalindromicsAtLetter, (void *)t); 
		if (rc) {
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
	}

	// Free attribute and wait for the other threads
	pthread_attr_destroy(&attr);
	for(t=0; t<numWorkers; t++) {
		rc = pthread_join(thread[t], &status);
		if (rc) {
			printf("ERROR; return code from pthread_join() is %d\n", rc);
			exit(-1);
		}
		printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
	}

	// We are going to print out the number of words
	// found by each thread and also sum them up
	for(i = 0; i < numWorkers; i++){
		printf("Main: Worker %i found %i palindromic pairs.\n",i,wordsFoundByThread[i]);
		totPalWords+=wordsFoundByThread[i];
	}

	// Print out the total number of palindromic pairs found
	printf("Main: Found %i total palindromic pairs overall.\n",totPalWords);
 	
 	// Exit and destroy any variables we have that we can delete
	printf("Main: program completed. Exiting.\n");
	pthread_mutex_destroy(&indexMutex);
	pthread_exit(NULL);
	for(i = 0; i < NUM_WORDS; i++){
		free(words[i]);
	}
	free(wordsFoundByThread);
	
	// Yay we are done.
	return 0;
}

/*
 * reverseWord
 * Reverses a given word
 *
 * inputs
 *		reversedWord: a pointer where the reversed word will be stored
 *		originalWord: a pointer to the word to be reversed
 */
void reverseWord(char* reversedWord, char* originalWord){

	// inits
	int i;
	int len = strlen(originalWord);

	// reverse the word
	for(i = 0; i < len; i++){
		reversedWord[len-i-1] = originalWord[i];
	}
	reversedWord[len] = '\0';
}

/*
 * doesWordExist 
 * Checks if word exists between index startI and 
 * endI. 
 *
 * inputs
 *		startI: The index in words array where to start searching from
 *		endI: The index in words array where to end searching from
 *		word: The word to search for in the words array
 */
int doesWordExist(int startI, int endI, char* word){

	// look at words from starting index to ending index
	while(startI < endI){

		// if strcmp is 0, the word was found in words array
		if(strcmp(words[startI],word)==0) return true;
		startI++;
	}

	return false;
}

/*
 * getFirstCharLower
 * For indexing purposes, gets the first character of the given
 * word and lowercases it
 *
 * inputs
 *		word: the words whose first character to return
 *
 * returns
 *		the first character lowercased
 */
char getFirstCharLower(char* word){

	// Retrieve the first letter
	char firstL = word[0];

	// If it's not already lowercase, lowercase it
	if(firstL < 'a'){
		firstL+=('a'-'A');
	}
	return firstL;
}

/*
 * findPalindromicsAtLetter
 * The function each thread runs, where it picks up a task from wordIndex. 
 * A task is defined as all the words for a starting letter. A task is 
 * taken out of the bag by incrementing wordIndex. The task itself 
 * is to find all palindromic words for each word in a letter group
 *
 * inputs
 *		t: The thread id
 */
void* findPalindromicsAtLetter(void* t){

	// initialize variables used by thread
	long tid = (long)t;
	int threadInd;
	char* reversedWord = malloc(sizeof(char)*MAX_WORD_LEN);
	int i, startI, endI, revStartI, revEndI;
	char firstL;
	int localCnt; 

	while(true){

		// localCnt is the count of palindromic words for this task 
		localCnt = 0;

		// The bag of tasks is shared data, and we don't want
		// two threads getting the same task at the same time,
		// so we lock it.
		pthread_mutex_lock(&indexMutex);

		// Bag of tasks is empty, so threads will finish.
		if(wordIndex == 26){
			pthread_mutex_unlock(&indexMutex);
			free(reversedWord);
			return NULL;
		}

		// Thread index is the task we are working on, so we can 
		// release wordIndex.
		threadInd = wordIndex;
		wordIndex++;
		pthread_mutex_unlock(&indexMutex);

		// startI and endI are the starting and ending
		// indices in the word array for some particular
		// starting letter. 
		startI = indices[threadInd];
		endI = indices[threadInd + 1];

		// For each word, reverse it, and look for the reversed
		// version in the rest of the dictionary file
		for(i=startI; i<endI; i++){
			reverseWord(reversedWord,words[i]);

			// firstL is the first character of the reversed word
			firstL = getFirstCharLower(reversedWord);

			// If the reversed word starts with a 'z', 
			// only look in the words array for words
			// starting with z. revStartI and revEndI
			// represent the indices in the words array
			// where the words start with firstL
			revStartI = indices[firstL-'a'];
			revEndI = indices[firstL-'a'+1];

			// only look from after the original word
			if(i>revStartI) revStartI = i;

			if(revStartI<=revEndI) {

				// check if the reversed word exists between revStartI 
				// and revEndI
				if(doesWordExist(revStartI,revEndI,reversedWord)){

					// Update the count for this task
					localCnt++;
				}
			}
		}

		// update total words found by thread
		wordsFoundByThread[tid]+=localCnt;
	}
}