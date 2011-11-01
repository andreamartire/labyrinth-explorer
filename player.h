#ifndef PLAYER_H
#define PLAYER_H

#include <GL/gl.h>
#include "maze.h"
#include "bbox.h"
#include "bbox_list.h"

typedef enum GAME { WIN, DEFAULT, LOSE } GAME;

struct player_t {
	GLfloat x, y, z;
	GLfloat size;
	GLfloat xRot, yRot;
	GLint firstPerson;
	GLint onFloor;
	GLfloat radius;
	GLfloat sensibility;
	bbox_t* box;
	bbox_list_t* bbox_list;
	GAME game;
};

typedef struct player_t player_t;

player_t* createPlayer(float, float, float, float, float, float, bbox_list_t*);

void showPlayer(player_t*);

#endif

