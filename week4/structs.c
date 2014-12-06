#include <string.h>
#include <stdio.h>
#include "structs.h"


struct book makebook(char* title, char* author, int pubyear){
	struct book newbook;
	strncpy(newbook.title, title, BOOKBUF);
	strncpy(newbook.author, author, BOOKBUF);
	newbook.pubyear = pubyear;
	return newbook;
	
}

struct book* remakebook ( struct book* oldbook, char* title, char* author, int pubyear){
	strncpy(oldbook->title, title, BOOKBUF);
	strncpy(oldbook->author, author, BOOKBUF);
	oldbook->pubyear = pubyear;
	return oldbook;
}

void set_book_pubyear(struct book* book, int newyear){
	if(newyear > 0){
		book->pubyear = newyear;
	}
}


int main(){
#define BOOKSHELF 8
	struct book book_shelf_1[] = { { "The C Language", "K&R", 1969 }, {}, \
		{"C for Dummies", "Gates", 1989} };
//	struct book book_shelf_2[BOOKSHELF] = { {} } ; // will intiliaze all elements to be empty
//	struct book book_shelf_3[BOOKSHELF]; // intial values are garbage values

	for(int i=0; i < sizeof(book_shelf_1)/sizeof(struct book); i++){
		printf("Book shelf 1, book %i\n", i);
		printf("title /%s/\n", book_shelf_1[i].title);
		printf("author /%s/\n", book_shelf_1[i].author);
		printf("year /%i/\n", book_shelf_1[i].pubyear);
	}

	struct book book_shelf_2 = calloc(BOOKSHELF, sizeof(struct book));
	if(!book_shelf_2){
		fputs("calloc failed", stderr);
		return 1;
	}
	struct book book_shelf_3 = malloc(BOOKSHELF*sizeof(struct book));
	if(!book_shelf_3){
		fputs("malloc failed", stderr);
		return 1;
	}

	struct book* book_shelf_4 = realloc(book_shelf_3, BOOKSHELF*BOOKSHELF*sizeof(struct book));
	if (!book_shelf_4){
		fputs("realloc error", stderr);
	}

	free(book_shelf_2);
	free(book_shelf_3);






	return 0;
}
