#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define DICT_CONTENT_SIZE 100000
#define GUESS_SIZE 2

int promptHangman(){
	/*************************************************
	Prompt user to play hangman
	**************************************************/
	printf("Do you want to play HANGMAN? Enter y/n: ");
	char* guess = malloc(sizeof(char)*GUESS_SIZE);
	scanf("%s", guess);
	// guess = "n";
	// printf("%s\n", guess);
		//if yes
		if(guess[0] == 'y'){
			printf("You said yes!\n");
			return 1;
		}
		else{
			printf("You didn't want to play :(. Quitting...\n");
			return 0;
		}

			//call playgame method with randomly selected word
			//loop to prompt guesses and print appropriate output

	//if no, exit
}

char** loadDictToStringArray(char* dictName) {
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
	//char dictContent[DICT_CONTENT_SIZE];
	char* dictContent = malloc(sizeof(char)*DICT_CONTENT_SIZE); //??????????????? doesnt work char*
	fgets(dictContent, DICT_CONTENT_SIZE, fileptr);

	fclose(fileptr);

	// printf("Dictionary contents \n %s", dictContent);


	/*************************************************
	Load dict file strings into memory in string array
	**************************************************/

	//char* words[DICT_CONTENT_SIZE]; //this gives warning
	char** words = malloc(sizeof(char*)*DICT_CONTENT_SIZE);
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


	return words;


}

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("Please provide a tab-delimited text file for the game to choose words from.\n");
		return 1;
	}
	if(argc > 2){
		printf("Only the first argument will be used as the text file.\n");
	}

	/*Seed future random calls with time*/
	srand(time(NULL));

	/*Make space for and load the dictionary filename
	into memory. Then declare array of strings so the
	call to loadDictToStringArray will put the words
	in the string array*/
	char* dictName = malloc(sizeof(char)*strlen(argv[1]));
	dictName = argv[1];
	//printf("%s\n", dictName);
	char** words; //IS THIS BAD??? should i be mallocing?
	words = loadDictToStringArray(dictName);



	int play = 0;
	play = promptHangman();
	if(play = 1){
		playHangman();
	}
	

	return 0;
}


	// _ _ _ _ 
	// |      |
	// |      0
	// |	  /|\\
	// |     / \\
	// |
	// |_ _ _ _






