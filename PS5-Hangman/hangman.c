#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define DICT_CONTENT_SIZE 100000

void loadDictToStringArray(char* dictName) {
	/************************************************
	Load dict file as string and split
	************************************************/

	/*file pointer variable marks place in the file*/
	FILE* fileptr; 
	/*open dictionary file with read access*/
	fileptr = fopen(dictName, "r");
	/*check that file pointer actually points to file*/
	if(fileptr == NULL){
		printf("Dictionary file not found. Quitting hangman.\n");
		//throw error
	}
	printf("Reading dictionary words from %s\n", dictName);

	/*fgets takes 3 args
	1- pointer to buffer where contents of string go (a char*)
	2- max size of contents stored at address of arg 1
	3- where to read from*/
	char dictContent[DICT_CONTENT_SIZE];
	//malloc(sizeof(char)*) ???
	fgets(dictContent, DICT_CONTENT_SIZE, fileptr);

	fclose(fileptr);

	// printf("Dictionary contents \n %s", dictContent);



	char* words[DICT_CONTENT_SIZE];
	char* p = strtok(dictContent, "\t");
	int i = 0;
	
	while(p != NULL){
		words[i++] = p;
		p = strtok(NULL, "\t");
	}
	// printf("HEY %s\n", words[0]);
	// printf("HEY %s\n", words[1]);
	// printf("HEY %s\n", words[2]);
	// printf("HEY %s\n", words[3]);

	printf("Your dictionary contains (%i) words!\n", i);


	/***********************************************/


	//return words;

	/*************************************************
	Load dict file strings into memory in string array
	**************************************************/
}

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("Please provide a tab-delimited text file for the game to choose words from.\n");
		return 1;
	}
	if(argc > 2){
		printf("Only the first argument will be used as the text file.\n");
	}

	srand(time(NULL));

	char* dictName = malloc(sizeof(char)*strlen(argv[1]));
	dictName = argv[1];
	//printf("%s\n", dictName);

	//char* words[DICT_CONTENT_SIZE] = loadDictToStringArray(dictName);
	loadDictToStringArray(dictName);
	


	//prompt user to play hangman 
		//if yes

			//call playgame method with randomly selected word
			//loop to prompt guesses and print appropriate output

	//if no, exit

	return 0;
}


	// _ _ _ _ 
	// |      |
	// |      0
	// |	  /|\\
	// |     / \\
	// |
	// |_ _ _ _






