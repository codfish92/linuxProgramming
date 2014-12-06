#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


//#define MYERR( fp, op) myerror(fp, __FUNCTION__, op)
#define MYERR( fp, ...) myerror(fp, __FUNCTION__, __VA_ARGS__ "")

#define BUFLEN 1024

void myerror(FILE* fp, const char* functionName, const char* op){

	char errbuf[BUFLEN];
	char* es = strerror_r( errno, errbuf, BUFLEN);
	if( strlen(op)){
		fprintf(fp, "%s (%s): %s\n", functionName, op, es);
	} else {
		fprintf(fp, "%s : %s\n", functionName, es);
	}
}


void show_error(void){
	printf("The errno is currently %d\n" , errno);
}


void testerror(void){
	if ( chdir ( "/does/notexit") ){
		MYERR(stderr, "chdir dne");
	}
}

int main(int argc, char* argv[]){

	FILE* fp = fopen("/etc/passwd", "w");
	if (!fp) {
		puts("shucks, couldn't write /etc/passwd");
		show_error();
		perror("main (write /etc/passwd)" );
		perror(NULL);
		perror("");
	}

	testerror();

	if( chdir("/does/not/exist" )) {
		MYERR(stderr , "chdir dne");
	}

	show_error();

	return 0;
}
