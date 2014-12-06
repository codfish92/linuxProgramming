/***
 * example interfaces to Linux signal handling
 */

#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "signal.h"

void signal_handler(int signum){

	if(signum == SIGUSR1){
		writeFlag = 1;
	}


}

