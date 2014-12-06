#include <string.h>

#include "swcutil.h"
void query_word(const char* const query, FILE* fp) {
	char* breadcrumb;
	size_t location = 0;
	const int BUFLEN = 4096;
	char buffer[BUFLEN];
	while ( fgets( buffer, BUFLEN, stdin) && !feof(stdin)){
		size_t thisline_was = strlen(buffer);
		size_t thistoken = location;
		char* theArg = buffer;
		char* token;
		while( (token = strtok_r( theArg, " \t\n\f\r", &breadcrumb)) ){
			if(!strcmp(query, token)){
				printf("The next token is '%s' at %lu\n", token, token-buffer+location);
			}
			theArg = NULL;
		}
		location += thisline_was;
	}
	return;
}
