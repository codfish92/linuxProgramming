/***
 * examples for date time coding
 */


#include <stdio.h>
#include <time.h>
#include <string.h>

#define BUFLEN 1024

/***
 * calendar time is a TIMESTAMP --- either time_t or struct tm
 */


// __FUNCTION__ is a preprocessor that will resolve to the current function ie get_onesec_resolution
time_t get_onesec_resolution( void )
{
	puts( __FUNCTION__ );

	time_t t;
	t = time( NULL ); // or time(&t);

	char buf[BUFLEN];
	// at least 26 bytes required (see manpage)
	ctime_r(&t,buf);
	
	fputs("The time() is ", stdout );
	fputs(buf, stdout);
	return t;
}

/***
 * User I/O in localtime,
 * Storage in zulu!
 */
int main( int argc, char* argv[] ) 
{
	time_t t = get_onesec_resolution();
	return 0;
}
