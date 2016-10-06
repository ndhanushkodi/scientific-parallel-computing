#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define DICT_CONTENT_SIZE 100000
#define GUESS_SIZE 3 //plus buffer space
#define ALPHA_SIZE 100 //so commas plus all lowercase letters

typedef struct DictInfo{
	int length;
	char** wordsArr;
} DictInfo;

typedef struct GameState{
	char* wordWithBlanks;
	char* guess;
	char* misses;
	int numMisses;
	int winLose; //0 is continue, 1 is lose, 2 is win
} GameState;
int promptHangman(DictInfo*, GameState*);

GameState* updateState(char* gameWord, GameState* state, char* guess){

	state->guess = guess;
	int hit = 0; //0 is miss, 1 is hit
	int starCount = 0; //if we have 0 stars at end, game state needs to be "win"
	int i;
	for(i=0; i<strlen(gameWord); i++){
		if(gameWord[i] == guess[0]){
			state->wordWithBlanks[i] = guess[0];
			hit = 1;
		}
		if(state->wordWithBlanks[i] == '*'){
			starCount++;
		}
	}
	if(hit == 0){
		/*TODOif(guess[0]) if misses doesnt contain guess, then do this
		*/
		state->misses[state->numMisses] = guess[0];
		state->numMisses++;
	}
	
	if(starCount == 0){
		state->winLose = 2; //got the word, win state
	}
	if(state->numMisses>15){
		state->winLose = 1; //lost the game, losing state
	}



	printf("*******DEBUGSTATE*******\n");
	//printf("DEBUGGameword: %s\n", gameWord);
	printf("Word: %s\n", state-> wordWithBlanks);
	printf("Starcount: %i\n", starCount);
	printf("Guess: %s\n", state-> guess);
	printf("Misses: %s\n", state-> misses);
	printf("Num Misses: %i\n", state-> numMisses);
	printf("ContinueWinLose: %i\n", state-> winLose);


	return state;
}

void playHangman(DictInfo* info, GameState* state){
	/*Randomly select a word from dictionary*/
	printf("Randomly selecting word from dictionary...\n");
	// printf("GOTTIEEEEEE%s\n", info->wordsArr[0] );
	// printf("GOTTIEEEEEE%i\n", info->length );
	int randIndex = rand()%info->length;
	char* gameWord = info->wordsArr[randIndex];
	int wordLen = strlen(info->wordsArr[randIndex]);
	printf("\n\nYour word is (%i) letters long!\n", wordLen);
	//printf("DEBUG: word was %s\n", info->wordsArr[randIndex]);
	
	/*Set up initial game state*/
	
	state->wordWithBlanks = calloc(wordLen+1, sizeof(char)); //
	state->wordWithBlanks[wordLen] = '\0';
	
	int i;
	for(i=0;i<wordLen;i++){
		state->wordWithBlanks[i] = '*';
	}
	printf("%s", state->wordWithBlanks);
	//printf("DEBUG LENGTH%i\n", (int) strlen(state->wordWithBlanks));
	state->guess = calloc(GUESS_SIZE, sizeof(char));
	state->misses = calloc(ALPHA_SIZE, sizeof(char));
	state->numMisses = 0;
	state->winLose = 0;

	char* guess = malloc(sizeof(char)*GUESS_SIZE);
	while(state->winLose == 0){
		/*Prompt guess*/
		printf("\n\nGuess a letter: ");
		scanf("%s", guess);
		

		//update state based on state->guess
		state = updateState(gameWord, state, guess);
		//print state
	}

	if(state->winLose == 1){
		printf("LOSERRRRRR\n");
		promptHangman(info, state);
	}
	if(state->winLose == 2){
		printf("WINNERRRRR\n");
		promptHangman(info, state);
	}

	free(state->wordWithBlanks);
	free(state->guess);
	free(state->misses);

}


int promptHangman(DictInfo* info, GameState* state){
	/*************************************************
	Prompt user to play hangman
	**************************************************/
	printf("_________________________________________________________\n");
	printf("\nDo you want to play HANGMAN? Enter y/n: ");
	
	char* playYN = malloc(sizeof(char)*GUESS_SIZE);
	scanf("%s", playYN);
	// guess = "n";
	// printf("%s\n", guess);
		//if yes
		if(playYN[0] == 'y'){
			printf("You said yes!\n\n\n");
			printf("_________________________________________________________\n");
			printf("\nPlaying HANGMAN\n\n");
			playHangman(info, state);
			free(playYN);
			return 1;
		}
		else{
			printf("You didn't want to play :(. Quitting...\n");
			free(playYN);
			return 0;
		}

	//if no, exit
}

DictInfo* loadDictToStringArray(char* dictName, DictInfo* info) {
	/************************************************
	Load dict file as string and split
	************************************************/

	/*file pointer variable marks place in the file*/
	FILE* fileptr; 
	/*open dictionary file with read access*/
	fileptr = fopen(dictName, "r");
	/*check that file pointer actually points to file*/
	if(fileptr == NULL){
		printf("\nDictionary file not found. Quitting hangman.\n");
		//throw error
	}
	printf("\n\n\n\n\nReading dictionary words from %s\n", dictName);

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
	char* p = strtok(dictContent, "\t"); //use strlen on p
	int i = 0;
	
	while(p != NULL){
		words[i++] = p;
		p = strtok(NULL, "\t");
	}

	/*HAD TO NULL OUT THE LAST CHAR OF THE LAST WORD BECAUSE
	FGETS FROM A TEXT FILE ADDS AN ASCII LF CHARACTER :(*/
	int lastWordLen = strlen(words[i-1]);
	words[i-1][lastWordLen-1] = 0;
	
	// printf("HEY %s\n", words[0]);
	// printf("HEY %s\n", words[1]);
	// printf("HEY %s\n", words[2]);
	 // printf("HEY %s\n", words[3]);
	 // printf("HEY %i\n", words[3][6]);
	 // printf("HEY %i\n", (int) strlen(words[3]));


	printf("Your dictionary contains (%i) words!\n", i);


	/***********************************************/


	
	info->length = i;
	info->wordsArr = words; 

	free(dictContent);


	return info;

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
	// char* dictName = malloc(sizeof(char)*strlen(argv[1]));
	// dictName = argv[1]; //???DOES THIS PUT NULL TERM FOR me? no need
	char* dictName = argv[1];
	//printf("%s\n", dictName);
	//char** words; //IS THIS BAD??? should i be mallocing?
	DictInfo* info = malloc(sizeof(struct DictInfo));
	info = loadDictToStringArray(dictName, info);

	GameState* state = malloc(sizeof(struct GameState));
	promptHangman(info, state);

	// int play = 0;
	// play = promptHangman();
	// if(play == 1){
	// 	playHangman(info);
	// }
	/*****************************************************
	??? THIS IS CORE DUMPING??????????????????????
	*****************************************************/
	//free(dictName);
	free(info->wordsArr);
	free(info);
	free(state);

	return 0;
}


	// _ _ _ _ 
	// |      |
	// |      0
	// |	  /|\\
	// |     / \\
	// |
	// |_ _ _ _


  // ____________
  //    |/      |
  //    |      (_)
  //    |      \|/
  //    |       |
  //    |      / \
  //    |
  //   _|___






