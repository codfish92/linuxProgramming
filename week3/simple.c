#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[]){
	int i = 0;
	FILE* fp = stderr;
	for(i = 0; i < argc; i++){
		fprintf(fp,"file number %d\n", fileno(fp));
		fputs(argv[i], fp);
		fputs("\n", fp);
		if(i%2 == 0){
			fp = stdout;
		}
		else{
			fp = stderr;
		}
		
	}

	return 0;
}
