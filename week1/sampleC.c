#include <stdio.h>

int main(int argc, char* argv[]){
	int c, width;
	//display prompt
	puts("Enter a character");
	//use shiftk in command mode to look up man page on library call
	if( (c=getc(stdin))==EOF) return 1;
	puts("Enter a width");
	if(scanf("%i", &width) != 1) return 1;
	if(width > 0 && width % 2 == 1) {
		printf("Read character '%c' and width %i\n", c, width);
		puts("Thank you for your support");
	} else{
		return 1;
	}
	return 0;
}
