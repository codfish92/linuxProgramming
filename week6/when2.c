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

// try first without the stringerizer
#define LZDIFF_D( field )  printf( "%-10s local%8d  zulu%8d\n", #field, lzone->field, zulu->field )
#define LZDIFF_LD( field )  printf( "%-10s local%8ld  zulu%8ld\n", #field, lzone->field, zulu->field )
#define LZDIFF_S( field )  printf( "%-10s local%8s  zulu%8s\n", #field, lzone->field, zulu->field )

// using the # in the cpre processor, if you prefix something with #, it wraps quotes around it. This is how 
// LZDIFF_D can be passed tm_sec and the function can still print tm_sec as text. the lzone->field will call the tm_sec of lzone

#define DBGINT(i) printf ("The value of " #i " is %i\n", i)
//will turn DBGINT(x) to 'the value of x is 10'. This makes poorman debugging good. 

void set_broken_time( time_t t, struct tm* const lzone, struct tm* const zulu )
{
	puts( __FUNCTION__ );

	gmtime_r( &t, zulu );
	localtime_r( &t, lzone );

	LZDIFF_D( tm_sec ); 	 /// sec [0,60] for leap sec
	LZDIFF_D( tm_min ); 	 /// min
	LZDIFF_D( tm_hour ); 	 /// 24 hour time
	LZDIFF_D( tm_mday ); 	 /// ORDINAL 1 = first day of month
	LZDIFF_D( tm_mon ); 	 /// index since jan
	LZDIFF_D( tm_year ); 	 /// index since 1900
	LZDIFF_D( tm_wday ); 	 /// index since sun
	LZDIFF_D( tm_yday ); 	 /// index since jan1
	LZDIFF_D( tm_isdst ); 	 /// is in DST  (1=in dst, 0=not in dsk, <0=use system config)
	LZDIFF_LD( tm_gmtoff );  /// number of seconds east of zulu when tm was valued, WITH isdst factored in
	LZDIFF_S( tm_zone );     /// tz name
}



/***
 * User I/O in localtime,
 * Storage in zulu!
 */
int main( int argc, char* argv[] ) 
{
	time_t t = get_onesec_resolution();
	struct tm local, zulu;
	//zulu is just utc
	//set broken time is the the time in a more human readable format ie month day year
	set_broken_time( t, &local, &zulu );

	return 0;
}


