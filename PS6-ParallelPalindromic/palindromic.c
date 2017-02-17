#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//count is system specific - this works for mac
#define NUM_WORDS 235886
#define MAX_WORD_LEN 30

char* words[NUM_WORDS];
int matched[NUM_WORDS];

void reverseWord(char* reversedWord, char* originalWord);

int main(int argc, char* argv[]){
	int i, j, len;
	int count = 0;
	//location is system specific - this works for mac 
	FILE* fp = fopen("wordsdict.txt", "r");
	if(!fp){
		printf("Unable to access dictionary\n");
		return -1;
	}

	//Read file into array of strings
	for(i = 0; i < NUM_WORDS; i++){
		words[i] = malloc(sizeof(char) * MAX_WORD_LEN);
		fgets(words[i], MAX_WORD_LEN, fp);
		len = strlen(words[i]);
		words[i][len-1] = '\0';
		printf("Read word: %s\n", words[i]);
	}
	
	for(i = 0; i < NUM_WORDS; i++){
		matched[i] = 0;
	}

	fclose(fp);
	
	char* reverse = malloc(sizeof(char) * MAX_WORD_LEN);
	for(i = 0; i < NUM_WORDS; i++){
		if(matched[i] == 0){//skip this word if found earlier
			//get reverse of words[i] and search for it
			reverseWord(reverse, words[i]);
			for(j = i; j< NUM_WORDS; j++){ //linear search ahead
				if(strcmp(words[j], reverse) == 0){
					count++;
					printf("Found %s and %s\n", words[i], reverse);
					matched[j] = 1;
				}
			}	
		}
	}
	
	printf("Found %d palindromic words\n", count);
	for(i = 0; i < NUM_WORDS; i++){
		free(words[i]);
	}
	free(reverse);
	
	return 0;
}

void reverseWord(char* reversedWord, char* originalWord){
	int i;
	int len = strlen(originalWord);
	for(i = 0; i < len; i++){
		reversedWord[len-i-1] = originalWord[i];
	}
	reversedWord[len] = '\0';
	//printf("%s reversed is %s\n", originalWord, reversedWord);
}