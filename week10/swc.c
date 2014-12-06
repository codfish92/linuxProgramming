#include "swcutil.h"
int main(int argc, char* argv[]){
	if (argc != 2) {
		puts("not enough arguments");
		return 1;
	}


	query_word(argv[1], stdin);

	return 0;

	
}
