#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "temp.h"


//set up fd daemon will use
FILE* ERRORLOG;
FILE* MESSAGELOG;

//globals for program(includes things like array pointer and length counts for rooms, reservtion ect)
char** roomArrayPtr;
int roomCount;

//func proto
void readRooms(char* filename);

int main(){
	ERRORLOG = fopen("tempErrorLog.log", "w");
	MESSAGELOG = fopen("tempMessageLog.log", "w");
	
	//check for errors opening the logs daemon will use.
	if(!ERRORLOG){
		fputs("ERROR opening ERRORLOG\n", stderr);
		exit(1);
	}
	if(!MESSAGELOG){
		fputs("ERROR opening MESSAGELOG\n", stderr);
		exit(1);
	}
	
	//deamonize
	//daemon(0,0)


	//after daemonize
}






void readRooms(char* filename){
	FILE* inputfile;
	inputfile = fopen(filename, "r");
	if(!inputfile){
		fputs("Error opening rooms input file\n", ERRORLOG);
	}
	int linecount = 0;
	char room[1024];
	//figure out how much to allocate
	while(fgets(room, ROOMBUF, inputfile)){
		if(room[0] != '\n' && room[0] != ' ' && room[0] != '\t'){
			linecount++;
		}
	}

	//allocate for array of strings
	char **roomArray = malloc(linecount*sizeof(char *));
	for(int i = 0; i < linecount; i++){
		roomArray[i] = malloc(ROOMBUF*sizeof(char));
	}
	//set file back to beginning
	fseek(inputfile, 0, SEEK_SET);
	linecount = 0;
	while(fgets(room, ROOMBUF, inputfile)){
		if(room[0] != '\n' && room[0] != ' ' && room[0] != '\t'){
			//sanitize the '\n' char
			for(int i = 0; i < ROOMBUF; i++){
				if(room[i] == '\n'){
					room[i] = '\0';
					break;
				}
			}
			strncpy(roomArray[linecount], room, ROOMBUF);// only copy room if not a new line
			linecount++;
		}
	}
	fclose(inputfile);
	roomArrayPtr = roomArray;
	roomCount = linecount;

}
