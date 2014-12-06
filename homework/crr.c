#include <errno.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>
#include <ncurses.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "crr.h"


#define CORNER '+'
#define VERT '|'
#define HORZ1 '-'
#define EDITSIZE 5
#define BUFFSIZE 1024


static reservationArrayType* arrayPtr;
int cursorRow = 0;
int writeFlag=0;
int reloadFlag=0;
char** roomArrayPtr;
int roomCount = 0;

void readRooms(char* filename);
void drawEditWindow(WINDOW* editWindow);
void drawDisplayWindow(WINDOW* display);
void drawMenu(WINDOW* display);
void drawSearch(WINDOW* display);
void drawCreate(WINDOW* display);
void drawKeyword(WINDOW* display);
void drawCancel(WINDOW* display);
void drawEditMenu(WINDOW* display);
int writeReservations(reservationArrayType array, char* filename);
reservationArrayType readReservations(char* filename);
void drawReservations(WINDOW* display, reservationArrayType array);
reservationType readInput(char* buffer);
time_t timeFromString(char* str);
void signal_handler(int signum);
int compareStart(const void* a, const void* b);
int compareRoom(void* a, void* b);
int searchReservation(reservationType reservation, char* searchTerm);
void displayTime(WINDOW* display);
void displayKeyword(WINDOW* display, char* keyword);
void displayCurrent(WINDOW* display);
WINDOW* makeDisplay();
WINDOW* makeEdit();
void displayReservationById(WINDOW* display, int id);

reservationType makeReservation(time_t start, time_t end, char* roomName, int roomId, char* description){
	reservationType newReserve;
	int foundRoom = 0;
	//make sure reservation has a endtime later than start
	//If the start is later than end, set name to failed, and roomid to be invalid
	if ( difftime(start, end) > 0){
		strncpy(newReserve.roomName, "failed", ROOMBUF);
		newReserve.roomId = -1;
	}
	else {
		for(int i =0; i < roomCount; i++){
			if(!strcasecmp(roomName, roomArrayPtr[i])){
				foundRoom = 1;
				break;
			}
		}
		if(foundRoom){
			strncpy(newReserve.roomName, roomName, ROOMBUF);
			newReserve.roomId = roomId;
		} 
		else{
			strncpy(newReserve.roomName, "failed", ROOMBUF);
			newReserve.roomId = -1;
		}
	}
	//REG7
	for(int i = 0; i < arrayPtr->length; i++){
		if(start < arrayPtr->at[i].end){
			if(start > arrayPtr->at[i].start){
				if(!strcasecmp(roomName, arrayPtr->at[i].roomName)){
					newReserve.roomId = -1; //mark as invalid
				}
			}
		}
	}
	strncpy(newReserve.description, description, ROOMBUF);
	//REG11
	struct tm utc;
	gmtime_r(&start, &utc);
	newReserve.start = mktime(&utc);
	gmtime_r(&end, &utc);
	newReserve.end = mktime(&utc);
	return newReserve;


}

//REG 4
void insertReservation(reservationArrayType* arrayPtr, reservationType reservation){
	// need to allocate more space for new entries and copy them all over. 
	if (arrayPtr->length == arrayPtr->maxLength){
		reservationType* newArray = malloc(arrayPtr->maxLength*2*sizeof(reservationType));
		arrayPtr->maxLength = arrayPtr->maxLength * 2;
		for(int i = 0; i< arrayPtr->length; i++){
			newArray[i] = arrayPtr->at[i];
		}
		free(arrayPtr->at);//copy complete, delete old array contents
		arrayPtr->at = newArray;//make sure to change pointer
	}
	//now we either had enough space to begin with, or just gave ourselves enoguh space, insert new entry
	strncpy(arrayPtr->at[arrayPtr->length].roomName, reservation.roomName, ROOMBUF);
	arrayPtr->at[arrayPtr->length].roomId = reservation.roomId;
	arrayPtr->at[arrayPtr->length].start = reservation.start;
	arrayPtr->at[arrayPtr->length].end = reservation.end;
	strncpy(arrayPtr->at[arrayPtr->length].description, reservation.description, ROOMBUF);
	arrayPtr->length++;
	fprintf(stderr, "Inserted the reservation: Room: %s, Roomid: %i, description: %s, start: %lu, end %lu", reservation.roomName, reservation.roomId, reservation.description, reservation.start, reservation.end); 
}

void deleteReservationByName(reservationArrayType array, char* name, time_t start){
	int removeIndex=-1;
	for(int i = 0; i < array.length; i++){
		//if we found the roomname, now check start time
		if(!strcasecmp(array.at[i].roomName, name)){
			if(array.at[i].start == start){
				removeIndex = i;
				array.at[i].roomId = -1; //make it not valid, incase it somehow escapes this method
				break;
			}

		}
	}

	//we never found the room to remove
	if(removeIndex == -1){
		return;
	}
	//shift every element in the array after the removal back one. 
	for(int i = removeIndex + 1; i < array.length; i++){
		array.at[i-1] = array.at[i]; 
	}
	array.length -= 1;

	//while it doesn't delete the element if you remove from end, the length paramter will pretend it has, so no other method will be able to access it. 
	

}
void deleteReservationById(reservationArrayType* array, int id){
	int removeIndex=-1;
	for(int i = 0; i < array->length; i++){
		if(array->at[i].roomId == id){
			removeIndex = i;
			array->at[i].roomId = -1;
			break;
		}
	}

	//we never found the room to remove
	if(removeIndex == -1){
		return;
	}
	//shift every element in the array after the removal back one. 
	for(int i = removeIndex + 1; i < array->length; i++){
		array->at[i-1] = array->at[i]; 
		array->at[i-1].roomId = array->at[i-1].roomId-1;
	}
	array->length -= 1;

	//while it doesn't delete the element if you remove from end, the length paramter will pretend it has, so no other method will be able to access it. 

}
int main(int argc, char* argv[]){

	if(argc != 3){
		fputs("Usage: ./crr	rooms.dat schedule.dat\n", stderr);
		exit(1);
	}

	
	//REG9
	signal(SIGUSR1, signal_handler);
	signal(SIGHUP, signal_handler);

	char buffer[BUFFSIZE];
	buffer[0] = '\0';
	int input;

	reservationArrayType array;
	//REG3 
	readRooms(argv[1]);
	array = readReservations(argv[2]);
	arrayPtr = &array;

	initscr();
	noecho();
	cbreak();
	keypad(stdscr,TRUE);
	curs_set(FALSE);
	wclear(stdscr);
	refresh();

	int maxX, maxY, i;
	WINDOW* display = makeDisplay();
	WINDOW* editWindow = makeEdit();
	refresh();
	

	drawDisplayWindow(display);
	drawMenu(display);
	drawEditWindow(editWindow);
		

	//REG2
	//printw("\n%i, %i\n", maxX, maxY);
	wrefresh(display);
	wrefresh(editWindow);
	int inSearch = 0;
	int inCreate = 0;
	int inCancel = 0;
	int inEdit = 0;
	int inEditMaking = 0;
	int inKeyword = 0;
	//listen for characters until the exit key
	while((input = getch()) != KEY_F(2)){
		//REG 9
		if(writeFlag ==1){
			writeReservations(array, argv[2]);
			writeFlag = 0;
			fputs("processed SIGUSR1\n", stderr);
		}
		//REG 9
		else if (reloadFlag ==1){
			inSearch = 0;
			inCreate = 0;
			inCancel = 0;
			inKeyword = 0;
			inEdit = 0;
			inEditMaking =0;
			writeFlag =0;
			reloadFlag =0;
			array = readReservations(argv[2]);
			wclear(display);
			wclear(editWindow);
			drawDisplayWindow(display);
			drawMenu(display);
			drawEditWindow(editWindow);
			buffer[0] = '\0';
			wrefresh(display);
			wrefresh(editWindow);
			fputs("processed SIGHUP\n", stderr);
		}
		else if(input == KEY_RESIZE){
			//REG2
			display = makeDisplay();
			editWindow = makeEdit();
			drawDisplayWindow(display);
			drawEditWindow(editWindow);
			wrefresh(display);
			wrefresh(editWindow);
			input = 10;
		}
		else if((int)input == 10) { //if the particualr char entered was the enter key
			if(inSearch == 0 && inCreate == 0 && inCancel == 0 && inEdit==0 && inEditMaking == 0){ //havent entered any menus
				drawMenu(display);
				wrefresh(display);
				wrefresh(editWindow);
				if(!strcasecmp(buffer, "search")){
					drawSearch(display);
					inSearch = 1;
				}
				else if (!strcasecmp(buffer, "cancel")){
					drawCancel(display);
					inCancel = 1;
				}
				else if (!strcasecmp(buffer, "create")){
					drawCreate(display);
					inCreate = 1;
				}
				else if (!strcasecmp(buffer, "edit")){
					drawEditMenu(display);
					inEdit = 1;
				}
				else if (!strcasecmp(buffer, "exit")){
					writeReservations(array, argv[2]);
					endwin();
					exit(0);
				}
				else if (!strcasecmp(buffer, "quit")){
					endwin();
					exit(0);
				}
				else{
					mvwprintw(display, cursorRow, 1, "%s\n", "unrecognized option");
				}	
			}
			else if(inCancel == 1){
				if(!strcasecmp(buffer, "back")){
					inCancel = 0;
					drawMenu(display);
				}
				else if(atoi(buffer) <= arrayPtr->length){
					deleteReservationById(arrayPtr, atoi(buffer));
					mvwprintw(display, cursorRow, 1, "deleted reservation %i\n", atoi(buffer));
					wrefresh(display);
					
				}
			}
			else if(inEditMaking>0){
				int i = inEditMaking-1;
				reservationType reservation = readInput(buffer);
				if(reservation.roomId == -1){
					mvwprintw(display, cursorRow, 1, "bad inputs\n");
				}
				else{
					strncpy(arrayPtr->at[i].roomName, reservation.roomName, ROOMBUF);
					arrayPtr->at[i].start = reservation.start;
					arrayPtr->at[i].end = reservation.end;
					strncpy(arrayPtr->at[i].description, reservation.description, ROOMBUF);
				}
				mvwprintw(display, cursorRow+1, 1, "press a key to continue\n");
				wrefresh(display);
				getch();
				inEditMaking=0;
				drawEditMenu(display);
			}
			else if(inEdit == 1){
				if(!strcasecmp(buffer, "back")){
					inEdit = 0; 
					drawMenu(display);
				}
				else if(atoi(buffer) <= arrayPtr->length){
					int i = atoi(buffer);
					inEditMaking = i+1;
					displayReservationById(display, inEditMaking);
					wrefresh(display);
					buffer[0] = '\0';
				}
			}
			else if (inKeyword == 1){
				displayKeyword(display, buffer);
				mvwprintw(display, 1, 1, "%s\n", "Press a key to continue");
				wrefresh(display);
				getch();
				inKeyword = 0;
				drawSearch(display);
			}
			else if (inSearch == 1){ //typed search
				if(!strcasecmp(buffer, "keyword")){
					mvwprintw(display, cursorRow, 1, "%s\n", "You have entered keyword");
					inKeyword = 1;
					drawKeyword(display);
				}
				else if (!strcasecmp(buffer, "time")){
					displayTime(display);
				}
				else if (!strcasecmp(buffer, "current")){
					displayCurrent(display);
				}
				else if (!strcasecmp(buffer, "back")){
					inSearch = 0;
					drawMenu(display);
				}
				else {
					mvwprintw(display, cursorRow, 1, "%s\n", "unreconginized option");
				}
				
			}
			else if (inCreate == 1){
				if(!strcasecmp(buffer, "back")){
					inCreate = 0;
					drawMenu(display);
				}
				else{
					reservationType newReserve = readInput(buffer);
					if(newReserve.roomId != -1){
						insertReservation(&array, newReserve);	
						mvwprintw(display, cursorRow, 1, "%s\n", "reservation made");
					}
					else{
						mvwprintw(display, cursorRow, 1, "%s\n", "Bad inputs, try again");
						fputs("got a bad reservation", stderr);
					}
				}
			}
			drawDisplayWindow(display);
			wrefresh(display);
			buffer[0] = '\0';
			drawEditWindow(editWindow);
		} else {
			sprintf(buffer,  "%s%c", buffer, input);
			mvwprintw(editWindow,1, 1, "%s", buffer);
		}
		wrefresh(editWindow);
	}
	wrefresh(editWindow);
	endwin();
	return 0;


}

void drawEditWindow(WINDOW* editWindow){
	wclear(editWindow);
	//make display for edit window
	int editX, editY;
	getmaxyx(editWindow, editY, editX);
	mvwaddch(editWindow, 0, 0, CORNER);
	mvwaddch(editWindow, 0, editX-1, CORNER);
	mvwaddch(editWindow, editY-1, editX-1, CORNER);
	mvwaddch(editWindow, editY-1, 0, CORNER);
	for(int i = 1; i < (editY-1); i++){
		mvwaddch(editWindow, i, 0, VERT);
		mvwaddch(editWindow, i, editX-1, VERT);
	}
	for(int i = 1; i < (editX-1); i++){
		mvwaddch(editWindow, 0, i, HORZ1);
		mvwaddch(editWindow, editY-1, i, HORZ1);
	}
	wrefresh(editWindow);

}


void drawDisplayWindow(WINDOW* display){
	//make display for primary menu window and display
	int maxY, maxX;
	getmaxyx(display, maxY, maxX);
	mvwaddch(display, 0, 0, CORNER);
	mvwaddch(display, 0, maxX-1, CORNER);
	mvwaddch(display, maxY-1, maxX-1, CORNER);
	mvwaddch(display, maxY-1, 0, CORNER);
	for(int i = 1; i< (maxY-1); i++){
		mvwaddch(display, i, 0, VERT);
		mvwaddch(display, i, maxX-1, VERT);
	}
	for(int i = 1; i< (maxX-1); i++){
		mvwaddch(display, 0, i, HORZ1);
		mvwaddch(display, maxY-1, i, HORZ1);
	}
	wrefresh(display);
}


void drawMenu(WINDOW* display){
	wclear(display);
	mvwprintw(display, 1, 1, "%s\n", "type 'search' to look through current current reservations");
	mvwprintw(display, 2, 1, "%s\n", "type 'create' to make a reservation");
	mvwprintw(display, 3, 1, "%s\n", "type 'cancel' to cancel a current reservation");
	mvwprintw(display, 4, 1, "%s\n", "type 'edit' to edit a reservation");
	mvwprintw(display, 5, 1, "%s\n", "type exit to exit and save");
	mvwprintw(display, 6, 1, "%s\n", "type quit to exit and without saving");
	cursorRow = 7;
	drawDisplayWindow(display);
}

void drawSearch(WINDOW* display){
	wclear(display);
	mvwprintw(display, 1, 1, "%s\n", "Type 'keyword', to print reservations with a specific key word");
	mvwprintw(display, 2, 1, "%s\n", "Type 'time' to search by the start time a room is reserved");
	mvwprintw(display, 3, 1, "%s\n", "Type 'current' to see all current reservations");
	mvwprintw(display, 4, 1, "%s\n", "Type 'back' to go back one menu");
	cursorRow = 5;
	drawDisplayWindow(display);

}

void drawCreate(WINDOW* display){
	wclear(display);
	mvwprintw(display, 1, 1, "%s\n", "Type the name of the room you want to reserve, start, end, and description in , seperated values");
	mvwprintw(display, 2, 1, "%s\n", "The times are enterned in the following format, yyyy/mm/dd/hh/min");
	mvwprintw(display, 3, 1, "%s\n", "An example entery is as follows");
	mvwprintw(display, 4, 1, "%s\n", "universe, 2014/11/24/00/30, 2015/03/03/14/00, doom bringners unite");
	mvwprintw(display, 5, 1, "%s\n", "will reserve the universe from 24th of november 2014 at 00:30 till 3rd of march 2015 2:00pm");
	mvwprintw(display, 6, 1, "%s\n", "The rooms available are as follows");
	for(int i =0; i < roomCount; i++){
		mvwprintw(display, 7+i, 1, "%s\n", roomArrayPtr[i]);
		
	}
	mvwprintw(display, 7+roomCount, 1, "%s\n", "Type back to stop creation");
	cursorRow = 8+roomCount;
	drawDisplayWindow(display);

	
}


int writeReservations(reservationArrayType array, char* filename) {
	
	FILE* outputFile;
	outputFile = fopen(filename, "wb");
	if(!outputFile){
		fputs("Could not open reservations.bin for writing", stderr);
		return 1;
	}
	for(int i = 0; i < array.length; i++){
		if(array.at[i].roomId == -1){
			continue; //skip this entry, because the room reservation is not valid. 
		}
		fwrite(&array.at[i], sizeof(reservationType), 1, outputFile);
	}

	fclose(outputFile);
	return 0;
	

}
//REG3
reservationArrayType readReservations(char* filename){
	reservationArrayType array;
	FILE* inputFile;

	inputFile = fopen(filename, "rb");

	//check file opened properly and can be manuvered in.
	if(!inputFile){
		//REG6
		if(errno == ENOENT){
			array.length = 0;
			array.maxLength = 10;
			reservationType* at = malloc(sizeof(reservationType)*array.maxLength);
			array.at = at;
			return array;
		}
		fputs("Cannot open binary file for reading.", stderr);
		array.maxLength = -1;
		return array;//return invalid array
	}
	if(fseek(inputFile, 0, SEEK_END)){
		fputs("Could not properly seek to end of binary file", stderr);
		array.maxLength = -1;
		return array;//return invalid array
	}

	//figure out how many entries there are in file. error if unexpected number appears
	//REG 8
	long bitLength = ftell(inputFile);
	fseek(inputFile, 0, SEEK_SET);//return cursor to begining of file
	if(bitLength%sizeof(reservationType)){
		fprintf(stderr, "Rouge data in inputfile, file is not a multiple of %lu", sizeof(reservationType)); 
		array.maxLength = -1;
		return array; //return invalid array
	}
	int length = bitLength/sizeof(reservationType);
	int maxLength = length*2; 

	reservationType* at = malloc(maxLength*sizeof(reservationType));



	//read in the files, ignoring reservations that are invlaid or are no longer relavent
	int position = 0;
	time_t now;
	now = time(NULL);
	for(int i = 0; i < length; i++){
		fread(&at[position], sizeof(reservationType), 1, inputFile); 
		if(difftime(at[position].end, now) < 0){
			fputs("time was less than", stderr);
			continue;//skip this entry, reservation is no longer relavent
		}
		if(at[position].roomId == -1){
			continue; //skip invalid entry that managed to get written
		}

		position++;
	}

	fclose(inputFile);
	array.at = at;
	array.length = position;
	array.maxLength = maxLength;
	return array;






}


void drawReservations(WINDOW* display, reservationArrayType array){
	reservationType current;
	mvwprintw(display, 4, 1, "array.lenth = %i", array.length);
	for(int i = 0; i < array.length; i++){
		current = array.at[i];
		mvwprintw(display, i+6, 1, "Room: %s, roomId: %i, Start: %lu, end: %lu, description: %s", current.roomName, current.roomId, current.start, current.end, current.description);
	}
}


reservationType readInput(char* buffer){
	reservationType newReserve;
	char string[ROOMBUF];
	strncpy(string, buffer, ROOMBUF);
	char* ptr;
	char roomName[ROOMBUF];
	char description[ROOMBUF];
	ptr = strtok(string, ",");
	if(ptr == NULL){
		newReserve = makeReservation(0, 0, "failed", -1, "bad"); 
		return newReserve;
	}
	time_t start;
	time_t end;
	strncpy(roomName, ptr, ROOMBUF);
	ptr = strtok(NULL, ",");
	if(ptr == NULL){
		newReserve = makeReservation(0, 0, "failed", -1, "bad"); 
		return newReserve;
	}
	start = timeFromString(ptr);
	ptr = strtok(NULL, ",");
	if(ptr == NULL){
		newReserve = makeReservation(0, 0, "failed", -1, "bad"); 
		return newReserve;
	}
	end = timeFromString(ptr);
	ptr = strtok(NULL, ",");
	if(ptr == NULL){
		newReserve = makeReservation(0, 0, "failed", -1, "bad"); 
		return newReserve;
	}
	strncpy(description, ptr, ROOMBUF); 
	newReserve = makeReservation(start, end, roomName, arrayPtr->length, description); 
	return newReserve;
}

time_t timeFromString(char* str){
	struct tm time;
	time_t local;
	time_t utc;
	time.tm_isdst = -1;
	char string[ROOMBUF];
	strncpy(string, str, ROOMBUF);
	strptime(string, "%Y/%m/%d/%H/%M", &time);
	fprintf(stderr, "year: %i, month: %i, day: %i, hour: %i, min: %i\n", time.tm_year, time.tm_mon, time.tm_mday, time.tm_hour, time.tm_min);
	strftime(string, ROOMBUF, "%Y/%m/%d/%H/%M", &time);
	fprintf(stderr, "Got the time %s\n", string);
	local = timelocal(&time);
	utc = local;
	return utc;
}
//REG9
void signal_handler(int signum){
	if(signum == SIGUSR1){
		writeFlag = 1;
	}
	else if (signum == SIGHUP){
		reloadFlag = 1;
	}
}

int compareStart(const void* a, const void* b){
	reservationType *tempa = (reservationType*)a;
	reservationType *tempb = (reservationType*)b;
	if(tempa->start == 0 && tempb->start > 0){
		return 1;
	}
	else if(tempb->start == 0 && tempa->start >0){
		return -1;
	}
	return tempa->start - tempb->start;
}
//REG5
int compareRoom(void* a, void* b){
	reservationType *tempa = (reservationType*)a;
	reservationType *tempb = (reservationType*)b;
	return strcasecmp(tempa->roomName, tempb->roomName);
}
int searchReservation(reservationType reservation, char* searchTerm){
	if(strstr(reservation.description, searchTerm) == NULL){
		return 0;
	}
	else{
		return 1;
	}
}
//REG3
void readRooms(char* filename){
	FILE* inputFile;
	inputFile = fopen(filename, "r");
	if(!inputFile){
		fputs("Error open rooms input file\n", stderr);
	}
	//get number of lines
	int lineCount = 0;
	char room[ROOMBUF];
	while(fgets(room, ROOMBUF, inputFile)){
		if(room[0] != '\n') //if the first character isn't a new line
			lineCount++;
	}
	//allocate space for rooms array
	char** roomArray = malloc(lineCount*sizeof(char *));
	for(int i = 0; i < lineCount; i++){
		roomArray[i] = malloc(ROOMBUF*sizeof(char));
	}
	fseek(inputFile, 0, SEEK_SET);//set the file to point to the start again
	lineCount = 0;
	while(fgets(room, ROOMBUF, inputFile)){
		if(room[0] != '\n'){
			//sanitize the newline character first
			for(int i = 0; i < ROOMBUF; i++){
				if(room[i] == '\n'){
					room[i] = '\0';
					break;
				}
			}
			strncpy(roomArray[lineCount], room, ROOMBUF);//copy the room if its not a newline
			lineCount++;
		}
	}
	fclose(inputFile);
	roomArrayPtr = roomArray;//set global pointer to array
	roomCount = lineCount;
}


void displayTime(WINDOW* display){
	char startStr[20];
	char endStr[20];
	//REG5
	qsort(arrayPtr->at, arrayPtr->length, sizeof(reservationType), compareStart);	
	for(int i = 0; i < arrayPtr->length; i++){
		strftime(startStr, 20, "%Y/%m/%d at %H:%M", localtime(&arrayPtr->at[i].start));
		strftime(endStr, 20, "%Y/%m/%d at %H:%M", localtime(&arrayPtr->at[i].end));
		mvwprintw(display, cursorRow+i, 1, "%s, %s, %s, %i, %s\n", arrayPtr->at[i].roomName, startStr, endStr, arrayPtr->at[i].roomId, arrayPtr->at[i].description);
	}

}

void displayKeyword(WINDOW* display, char* keyword){
	int count = 0;
	char startStr[20];
	char endStr[20];
	for(int i = 0; i < arrayPtr->length; i++){
		if(searchReservation(arrayPtr->at[i], keyword)){
			strftime(startStr, 20, "%Y/%m/%d at %H:%M", localtime(&arrayPtr->at[i].start));
			strftime(endStr, 20, "%Y/%m/%d at %H:%M", localtime(&arrayPtr->at[i].end));
			mvwprintw(display, cursorRow+count, 1, "%s, %s, %s, %i, %s\n", arrayPtr->at[i].roomName, startStr, endStr, arrayPtr->at[i].roomId, arrayPtr->at[i].description);
			count++;
		}
	}
}

void drawKeyword(WINDOW* display){
	wclear(display);	
	mvwprintw(display, 1, 1, "%s\n","Type the keyword you would like search, all reservations with that word will be displayed");
	drawDisplayWindow(display);
	cursorRow = 2;
}

void displayCurrent(WINDOW* display){
	int count = 0;
	time_t now = time(NULL);
	mvwprintw(display, cursorRow, 1, "%s\n", "These are the reservations that are active as of today");
	char startStr[20];
	char endStr[20];
	for(int i = 0; i< arrayPtr->length; i++){
		if(arrayPtr->at[i].start < now){
			if(arrayPtr->at[i].end > now){
				strftime(startStr, 20, "%Y/%m/%d at %H:%M", localtime(&arrayPtr->at[i].start));
				strftime(endStr, 20, "%Y/%m/%d at %H:%M", localtime(&arrayPtr->at[i].end));
				mvwprintw(display, cursorRow+count+1, 1, "%s, From %s to %s, %i, %s", arrayPtr->at[i].roomName, startStr, endStr, arrayPtr->at[i].roomId, arrayPtr->at[i].description);
				count++;
			}
		}
	}
}

//REG2
WINDOW* makeDisplay(){
	wclear(stdscr);
	int maxX, maxY;
	getmaxyx(stdscr, maxY, maxX);
	WINDOW *display = newwin(maxY-EDITSIZE, maxX, 0, 0);
	return display;
	
}
//REG2
WINDOW* makeEdit(){
	int maxX, maxY;
	getmaxyx(stdscr, maxY, maxX);
	WINDOW *editWindow = newwin(EDITSIZE-1,maxX, maxY-EDITSIZE, 0);
	return editWindow;
}

void drawCancel(WINDOW* display){
	wclear(display);
	mvwprintw(display, 1, 1, "%s\n", "Enter the id of the reservation to cancel");
	mvwprintw(display, 2, 1, "%s\n", "type 'back' to go back to the main menu");
	cursorRow = 3;
	drawDisplayWindow(display);

}

void drawEditMenu(WINDOW* display){
	wclear(display);
	mvwprintw(display, 1, 1, "%s\n", "Enter the id of the reservation you want to edit");
	mvwprintw(display, 2, 1, "%s\n", "type 'back' to go back to the main menu");
	cursorRow = 3;
	drawDisplayWindow(display);

}

void displayReservationById(WINDOW* display, int id){
	int i = id-1;
	char startStr[20];
	char endStr[20];
	strftime(startStr, 20, "%Y/%m/%d at %H:%M", localtime(&arrayPtr->at[i].start));
	strftime(endStr, 20, "%Y/%m/%d at %H:%M", localtime(&arrayPtr->at[i].end));
	mvwprintw(display, cursorRow, 1, "using the same syntax as create, enter the new parameters for this reservation\n");
	mvwprintw(display, cursorRow+1, 1, "%s, id: %i, From %s to %s, %s", arrayPtr->at[i].roomName, arrayPtr->at[i].roomId, startStr, endStr, arrayPtr->at[i].description); 
	wrefresh(display);

}
