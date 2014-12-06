#include <stdio.h>
#include <regex.h>
#include <stdlib.h>
int* fillElements(int* x);
int main(){

	int *x;
	x = malloc(10*sizeof(int));
	x = fillElements(x);
}
int* fillElements(int* x){
	int i;
	for(i = 0; i < 10; i++){
		x[i] = i;
	}
	int count = sizeof(x)/sizeof(int);
	printf("%i\n", count);
}
