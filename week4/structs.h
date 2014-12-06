#ifndef STRUCTS_H
#define STRUCTS_H

#define BOOKBUF 1024

//way 1
struct book{
	char title[BOOKBUF];
	char author[BOOKBUF];
	int pubyear;


};
//way 1.5
typedef struct book BookType;

//way 2
typedef struct biography{
	char title[BOOKBUF];
	char author[BOOKBUF];
	int pubyear;
	char about[BOOKBUF];
} BiographyType;

struct book makebook(char* title, char* author, int pubyear);
BookType make_another_book(char* title, char* author, int pubyear);
struct book* remakebook(struct book* oldbook, char* title, char* author, int pubyer);
BiographyType makebiograph();

void set_book_pubyear(struct book* book, int newyear);


#define SONGBUFF 1024

struct song{
	char title[SONGBUFF];
	char artist[SONGBUFF];
	int pubyear;
	float ranking;
};

#define MOVIEBUFF 1024

struct movie{
	char title[MOVIEBUFF];
	int pubyear;
	float ranking;
};

struct song makesong(char* title, char* artist, int pubyear, float ranking);
struct movie makemovie(char* title, int pubyear, float ranking);
void set_song_ranking(struct song* song, float ranking);
void set_movie_ranking(struct movie* movie, float ranking);



#endif
