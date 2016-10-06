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

typedef struct Counts{
	int charCount;
	int wordCount;
} Counts;

typedef struct GameState{
	char* wordWithBlanks;
	char* guess;
	char* misses;
	int numMisses;
	int winLose; //0 is continue, 1 is lose, 2 is win
} GameState;


/*Print out the Word, current guess, misses, the number of misses
so far, and the gallows*/
void printState(GameState* state){
	
	//printf("DEBUGGameword: %s\n", gameWord);
	printf("Word: %s\n", state-> wordWithBlanks);
	
	printf("Guess: %s\n", state-> guess);
	printf("Misses: %s\n", state-> misses);

	printf("Number of misses so Far: %i\n\n", state-> numMisses);
	switch(state->numMisses){
		case 0:
		break;

		case 1:
		printf("   |\n");
		printf("  _|___\n");
		break;

		case 2:
		printf("____________\n");
		printf("   |/\n");
		printf("   |\n");
		printf("   |\n");
		printf("   |\n");
		printf("   |\n");
		printf("   |\n");
		printf("  _|___\n");
		break;

		case 3:
		printf("____________\n");
		printf("   |/      |\n");
		printf("   |\n");
		printf("   |\n");
		printf("   |\n");
		printf("   |\n");
		printf("   |\n");
		printf("  _|___\n");
		break;

		case 4:
		printf("____________\n");
		printf("   |/      |\n");
		printf("   |      (_)\n");
		printf("   |\n");
		printf("   |\n");
		printf("   |\n");
		printf("   |\n");
		printf("  _|___\n");
		break;

		case 5:
		printf("____________\n");
		printf("   |/      |\n");
		printf("   |      (_)\n");
		printf("   |       |\n");
		printf("   |\n");
		printf("   |\n");
		printf("   |\n");
		printf("  _|___\n");
		break;

		case 6:
		printf("____________\n");
		printf("   |/      |\n");
		printf("   |      (_)\n");
		printf("   |      \\|/\n");
		printf("   |\n");
		printf("   |\n");
		printf("   |\n");
		printf("  _|___\n");
		break;

		case 7:
		printf("____________\n");
		printf("   |/      |\n");
		printf("   |      (_)\n");
		printf("   |      \\|/\n");
		printf("   |       |\n");
		printf("   |\n");
		printf("   |\n");
		printf("  _|___\n");
		break;

		case 8:
		printf("____________\n");
		printf("   |/      |\n");
		printf("   |      (_)\n");
		printf("   |      \\|/\n");
		printf("   |       |\n");
		printf("   |      /\n");
		printf("   |\n");
		printf("  _|___\n");
		break;

		case 9:
		printf("____________\n");
		printf("   |/      |\n");
		printf("   |      (_)\n");
		printf("   |      \\|/\n");
		printf("   |       |\n");
		printf("   |      / \\\n");
		printf("   |\n");
		printf("  _|___\n");
		break;

	}
}

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
	if(state->numMisses>8){
		state->winLose = 1; //lost the game, losing state
	}


	printf("_____________________________________________\n");
	printf("Starcount: %i\n", starCount);


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

		/*update state based on guess*/
		state = updateState(gameWord, state, guess);
		//print state
		printState(state);
	}

	free(state->wordWithBlanks);
	free(state->guess);
	free(state->misses);

	if(state->winLose == 1){
		printf("\n\nYOU LOST!!\n");
		//promptHangman(info, state);
	}
	if(state->winLose == 2){
		printf("\n\nYOU WON!\n");
		//promptHangman(info, state);
	}
}

/*Returns 1 if the user wants to play hangman
Returns 0 if the user doesn't want to play hangman*/
int promptHangman(){
	/*************************************************
	Prompt user to play hangman
	**************************************************/
	printf("_________________________________________________________\n");
	printf("\nDo you want to play HANGMAN? Enter y/n: ");
	
	/*Make space for the user's one character input*/
	char* playYN = malloc(sizeof(char)*GUESS_SIZE);
	scanf("%s", playYN);

		//if user says yes
		if(playYN[0] == 'y'){
			free(playYN);
			printf("You said yes!\n\n\n");
			printf("_________________________________________________________\n");
			printf("\nPlaying HANGMAN\n\n");
			return 1;
		}

		else{
			free(playYN);
			printf("You didn't want to play :(. Quitting...\n");
			return 0;
		}
}


/*I referenced this stackoverflow
http://stackoverflow.com/questions/7374062/how-do-i-count-the-number-of-words-in-a-text-file-using-c
and tried to make sure I understood each line of code that I used from it.*/
Counts* wordCount(FILE* fileptr, Counts* cts){
	//initialize contents of Counts* struct pointer
	cts->wordCount = 0;
	cts->charCount = 0;

	char c;
	c = fgetc(fileptr);
	while(c != EOF){ //walk through every char
		cts->charCount++; //increment char counter
		if(c == '\t')
			cts->wordCount++; //when we see a tab delimeter, increment word count
		c = fgetc(fileptr);
	}
	fclose(fileptr);
	cts->wordCount = cts->wordCount+1; //to account for the last word, which doesn't have a tab after it
	return cts; 
}

/*Takes in a filename, and returns a read only
file pointer to the beginning of the file.*/
FILE* getReadFilePointer(char* fileName){
	/*file pointer variable marks place in the file*/
	FILE* fileptr; 
	/*open dictionary file with read access*/
	fileptr = fopen(fileName, "r");
	/*check that file pointer actually points to file*/
	if(fileptr == NULL){
		printf("\nDictionary file not found. Quitting hangman.\n");
		//throw error
	}
	return fileptr;
}

/*Returns a DictInfo* with length of the dictionary in number of words
and all of the words in a string array.*/
DictInfo* loadDictToStringArray(char* dictName, DictInfo* info) {
	/************************************************
	Load dict file as string and split
	************************************************/
	printf("\n\n\n\n\nReading dictionary words from %s\n", dictName);

	//store word and character counts in Counts* struct pointer
	/*NOTE: my hangman would word if I actually used
	counts->charCount for the char* dictContent and counts->wordCount
	for the char**words, but it would core dump after a few games.
	So, I ended up leaving it to use DICT_CONTENT_SIZE*/
	FILE* f = getReadFilePointer(dictName);
	Counts* counts = malloc(sizeof(struct Counts));
	counts = wordCount(f, counts);


	/*fgets takes 3 args
	1- pointer to buffer where contents of string go (a char*)
	2- max size of contents stored at address of arg 1
	3- where to read from*/
	FILE* fileptr = getReadFilePointer(dictName);
	char* dictContent = malloc(sizeof(char)*DICT_CONTENT_SIZE); //??????????????? doesnt work char*
	fgets(dictContent, DICT_CONTENT_SIZE, fileptr);

	fclose(fileptr);

	// printf("DEBUG: Dictionary contents \n %s", dictContent);


	/*************************************************
	Load dict file strings into memory in string array
	**************************************************/
	/*Unfortunately had to us DICT_CONTENT_SIZE as described in the
	 note above.*/
	char** words = malloc(sizeof(char*)*DICT_CONTENT_SIZE); 
	/*get pointer to first word*/
	char* p = strtok(dictContent, "\t"); //use strlen on p to see that null terminator is taken care of
	int i = 0;
	
	while(p != NULL){ //continue getting pointers to words
		words[i++] = p; //store the words
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
	
	info->length = i;
	info->wordsArr = words; 

	// printf("DEBUGHEY 2%s\n", info->wordsArr[0]);
	// printf("DEBUGHEY 2%s\n", info->wordsArr[1]);
	// printf("DEBUGHEY 2%s\n", info->wordsArr[2]);
	//  printf("DEBUGHEY 2%s\n", info->wordsArr[3]);
	//  printf("DEBUG HEY 2%i\n", info->wordsArr[3][6]);
	//  printf("DEBUG HEY 2%i\n", (int) strlen(info->wordsArr[3]));

	free(dictContent);
	free(counts);


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
	into memory.*/
	char* dictName = argv[1];

	/*Then declare info struct and make space for it. 
	It will contain number of words in the dictionary and 
	the array of words. */
	DictInfo* info = malloc(sizeof(struct DictInfo));
	/*call the method that will take the text file and put it
	into "info" format*/
	info = loadDictToStringArray(dictName, info);

	/*state will hold important game state info that will
	be altered during each guess in hangman*/
	GameState* state = malloc(sizeof(struct GameState));
	/*prompt user to play hangman*/
	int play = promptHangman();

	/*After every game we prompt the user to play hangman,
	and we keep going until the user says no*/
	while(play == 1){ 
		playHangman(info, state);
		play = promptHangman();
	}

	/*Free things inside info and state at the end of the program*/
	free(info->wordsArr);
	free(info);
	free(state);
	

	return 0;
}
