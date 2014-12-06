#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		puts("not enought input arguments\n");
		return 1;
	}
	
	FILE* dict;
	char buffer[1024];
	long myoffset = 0;
	long theiroffset = 0;
	if ( (dict = fopen(argv[1], "r")) == NULL){
		fprintf(stderr, "Cannot open '%s'\n", argv[1]);
		return 1;
	}

	if(( sscanf(argv[2], "%li", &theiroffset))!=1){
		fprintf(stderr, "Could not read the offset amount '%s'\n", argv[2]);
		return 1;
	}

	while(myoffset < theiroffset){
		if(fgets(buffer, 1024, dict) != buffer) {
			break;	
		}
		myoffset+= strlen(buffer);
		if(myoffset >= theiroffset){
			printf("%li %li %li\n", theiroffset, myoffset, strlen(buffer));
		    if(buffer[theiroffset-myoffset+strlen(buffer)]!=10){
				fputs(buffer, stdout);
			}
				
		}
	}

	fclose(dict);
	return 0;
}
