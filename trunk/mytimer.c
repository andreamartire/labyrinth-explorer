#include "mytimer.h"

#include <stdlib.h>
#include <stdio.h>

mytimer_t* createTimer(int countdown){
	mytimer_t *mytimer = malloc(sizeof(mytimer_t));
	mytimer->begin = time(0); //start the timer
	mytimer->current = mytimer->begin;
	mytimer->elapsed = 0;
	mytimer->countdown = countdown;
	return mytimer;
}

void updateTimer(mytimer_t* mytimer){
	mytimer->current = time(0);
	mytimer->elapsed = mytimer->current - mytimer->begin;
	mytimer->residual = mytimer->countdown - mytimer->elapsed;
	if(mytimer->residual < 0)
		mytimer->residual = 0;
}

char* getTimer(mytimer_t* mytimer){
	char* tmp = malloc(sizeof(char)*20);
	int sec = mytimer->residual;
	
	//~ if( (sec % 60) != 0 )
		//~ sprintf(tmp, "%d",  sec % 60);
	//~ else
		//~ sprintf(tmp, "0");
	//~ sec -= sec % 60;
	
	sprintf(tmp, "%d", sec);
	//todo minutes
	return tmp;
}
