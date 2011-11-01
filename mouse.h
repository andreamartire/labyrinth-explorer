#ifndef MOUSE_H
#define MOUSE_H

#include <GL/gl.h>
#include "player.h"

struct mouse_t {
	GLint x_old;
	GLint y_old;
	player_t* player;
};

typedef struct mouse_t mouse_t;

mouse_t* createMouse(player_t*);

void updateMouse(mouse_t*, GLint, GLint);

#endif

