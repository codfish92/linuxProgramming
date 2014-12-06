#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	/*
	char aChar = 'a', anotherChar = '\n';
	puts("My name is mud");


	// a string in c is simply an array of chars
	char aString[] = "My name is mud";
	puts(aString);

	printf("The string is %lu bytes long? %s\n", strlen(aString), aString);

	aString[11] = '\0';

	printf("The string is %lu bytes long? %s\n", strlen(aString), aString);

	aString[11] = 'M';

	printf("The string is %lu bytes long? %s\n", strlen(aString), aString);

	*/
	int i = 0;

	char* breadCrumb;


	for (i =0; i < argc; i++) {
		char* theArg = argv[i];
		char* token;
		while( (token = strtok_r(theArg, " \t", &breadCrumb) ) ){
			printf("The next token is %s\n", token);
			theArg = NULL;
		}

	}




	return 0;
}
