#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include "attachable_debugger.h"
int main()
{
	attachable_debugger();

	printf( "pid: %d\n", getpid() );
	char c[3] = { 0, '\n', '\0' };
	c[0] = getc(stdin);
	fputs( c, stdout );
	return 0;
}
