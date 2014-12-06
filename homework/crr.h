#ifndef CRR
#define CRR
#include <time.h>
#define ROOMBUF 1024

typedef struct reservation{
	time_t start;
	time_t end;
	char roomName[ROOMBUF];
	char description[ROOMBUF];
	int roomId; 
} reservationType;


typedef struct reservationArray{
	reservationType* at; //this is to make it seem comparable to other language array structure syntax. at is the array of reservation pointers. 
	int length;
	int maxLength;
} reservationArrayType;

reservationType makeReservation(time_t start, time_t end, char* roomName, int roomId, char* description);
void deleteReservationByName(reservationArrayType array, char* name, time_t start);
void deleteReservationById(reservationArrayType* array, int id);
void insertReservation(reservationArrayType* arrayPtr, reservationType reservation);





#endif
