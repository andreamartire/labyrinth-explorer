#include "mouse.h"

#include <stdlib.h>

mouse_t* createMouse(player_t* player){
	mouse_t *mouse = malloc(sizeof(mouse_t));
	mouse->x_old = 1;
	mouse->y_old = 1;
	mouse->player = player;
	return mouse;
}

void updateMouse(mouse_t* mouse, GLint x, GLint y){
	
	if ( x<10 ){
		glutWarpPointer(200, y);
		mouse->x_old = 200;
	}
	else if ( x>200 ){
		glutWarpPointer(10, y);
		mouse->x_old = 10;
	}
	else if ( y<10 ) {
		glutWarpPointer(x, 200);
		mouse->y_old = 200;
	}
	else if ( y>200 ) {
		glutWarpPointer(x, 10);
		mouse->y_old = 10;
	}
	else{
		float x_diff = (float) x - mouse->x_old;
		float y_diff = (float) y - mouse->y_old;
		mouse->x_old = x;
		mouse->y_old = y;
		
		mouse->player->xRot += y_diff;
		mouse->player->yRot += x_diff;
			
		mouse->player->xRot = (GLfloat)((const int)mouse->player->xRot % 360);
		mouse->player->yRot = (GLfloat)((const int)mouse->player->yRot % 360);
		
		if(mouse->player->xRot > 90)
			mouse->player->xRot = 90;
		if(mouse->player->xRot < 0)
			mouse->player->xRot = 0;
	}
}
