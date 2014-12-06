#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	if (argc != 2) {
		puts("not enough arguments");
		return 1;
	}

	char* breadcrumb;
	char* search = argv[1];
	size_t location = 0;
	int i;
	const int BUFLEN = 4096;
	char buffer[BUFLEN];
	while ( fgets( buffer, BUFLEN, stdin) && !feof(stdin)){
		size_t thisline_was = strlen(buffer);
		size_t thistoken = location;
		char* theArg = buffer;
		char* token;
		while( (token = strtok_r( theArg, " \t\n\f\r", &breadcrumb)) ){
			printf("The next token is '%s' at %lu\n", token, token-buffer+location);
			if(!strcasecmp(token, search)){
			}
			thistoken += strlen(token)+1;
			theArg = NULL;
		}
		location += thisline_was;
	}
	return 0;

	
}
