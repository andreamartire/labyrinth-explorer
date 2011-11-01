#ifndef TIMER_H
#define TIMER_H

#include <GL/gl.h>
#include <time.h>

struct mytimer_t {
	double begin;
	double current;
	double elapsed;
	int countdown;
	double residual;
};

typedef struct mytimer_t mytimer_t;

mytimer_t* createTimer();

void updateTimer(mytimer_t*);

char* getTimer(mytimer_t*);

#endif

