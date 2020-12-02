// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
void stop_handler(int signum) {
	int pID = getpid();
	kill(pID, SIGTSTP);
	//how to get the info from the signal?
	//use exeExternal as BG with flag to indicate this was stopped. i think.
}

void continte_handler(int signum) {
	int pID = getpid();
	kill(pID, SIGINT);
}

