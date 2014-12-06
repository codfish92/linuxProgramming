#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{

	if(argc < 2){
		puts("Please provide a word to play the game with");
		return 1;
	}



	
    const int HANGMAN=7;
    char hangman[][14] = {
        " +-----+    \n",
        " |     ;    \n",
        " |          \n",
        " |          \n",
        " |           \n",
        " |          \n",
        "==========  \n",
        };

    const int PARTS=6;
    const char lineparts[] = { 2, 2, 2, 3, 4, 4 };
    const char charparts[] = { 7, 6, 8, 7, 6, 8 };
    const char bodyparts[] = "O__|/\\";
    int i, M=0;
	char* targetWord = argv[1];
	int length = strlen(targetWord);
	char showedWord[length+1];
	for(i = 0; i < length; i++){
		if(isalpha(targetWord[i]) == 0){
			puts("non alpha letter detected, please try again");
			return 1;
		}
		showedWord[i] = '_';
	}
	char guess;



	//intial display
	hangman[ lineparts[M-1] ][ charparts[M-1] ] = bodyparts[M-1] ;
	/** display */
	for( i=0; i<HANGMAN; i++ ) {
		fputs( hangman[i], stdout );
	}


	printf("%s\n", showedWord);
	puts("Please enter a letter.");
	guess = fgetc(stdin);
	while (guess){
		//check if char is in string
		if(strchr(targetWord, guess)!=NULL){
			//still have to iterate through due to multiple occurances
			for(i = 0; i < length; i++){
				if (targetWord[i] == guess){
					showedWord[i] = guess;
				}
			}
			//If no more _ chars exist in the showed word, game is won
			if(strchr(showedWord, '_')){
				printf("You Won, the word was %s: You had %i missed guesses\n", targetWord, M);
			}
		}
		else{
			printf("'%s'\n", strchr(targetWord, guess));
			M++;
		}
		hangman[ lineparts[M-1] ][ charparts[M-1] ] = bodyparts[M-1] ;
		/** display */
		for( i=0; i<HANGMAN; i++ ) {
			fputs( hangman[i], stdout );
    	}
		puts(showedWord);
		if(M>=7){
			printf("Game over: the word you were trying to guess was %s\n", targetWord);
			return 0;
		}
		puts("Please enter a letter.");
		guess = fgetc(stdin);


	}




    return 0;
}
