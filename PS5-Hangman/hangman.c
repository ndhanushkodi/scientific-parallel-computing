#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc < 2){
		printf("Please provide a tab-delimited text file for the game to choose words from.\n");
		return 1;
	}
	if(argc > 2){
		printf("Only the first argument will be used as the text file.\n");
	}


	char* dictName = malloc(sizeof(char)*strlen(argv[1]));
	dictName = argv[1];
	printf("%s\n", dictName);
	//load dict file as string and split

	//load dict file strings into memory in string array


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






