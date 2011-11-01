#ifndef MAZE_H
#define MAZE_H

#include <GL/gl.h>
#include "mytimer.h"

typedef enum MAZE_TYPE { STANDARD, CRAZY } MAZE_TYPE;

struct maze_t {
	GLint** maze;
	mytimer_t *mytimer;
	GLint rows;
	GLint cols;
	MAZE_TYPE type;
	MAZE_TYPE oldtype;
	GLint viewGraphic;
	GLint doubled;
	GLint bounded;
	GLint xs;
	GLint ys;
	GLint xf;
	GLint yf;
};

typedef struct maze_t maze_t;

maze_t* loadMaze(char*, MAZE_TYPE);

void showMaze(maze_t*);

void initPositionMaze(maze_t*);

#endif

