#include "player.h"
#include "bbox.h"
#include "bbox_list.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

player_t* createPlayer( float x, float y, float z, float xRot, float yRot, float size, bbox_list_t* list ){
	player_t *player = malloc(sizeof(player_t));
	player->x = x;
	player->y = y;
	player->z = z;
	player->size = size;
	player->xRot = xRot;
	player->yRot = yRot;
	player->firstPerson = 0;
	player->radius = 10;
	player->sensibility = 5;
	player->onFloor = 1;
	player->bbox_list = list;
	player->game = DEFAULT;
	float off = size;
	player->box = createBBox(x-off, y-off, z-off, x-off, y-off, z+off,
							 x+off, y-off, z+off, x+off, y-off, z-off,
							 x-off, y+off, z-off, x-off, y+off, z+off,
							 x+off, y+off, z+off, x+off, y+off, z-off);
	list->player_box = player->box;
	return player;
}

void showPlayer(player_t* player){
	if(!player->firstPerson)
		glTranslatef(0.0f, 0.0f, -player->radius);
		
	// Move object back and do in place rotation
	glRotatef(player->xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(player->yRot, 0.0f, 1.0f, 0.0f);
	if(!player->firstPerson)
		glutSolidSphere(player->size, 30, 30);
}

int notCollision(GLint iTest, GLint jTest, maze_t* maze){
	//~ printf("check %d,%d = %d\n", iTest, jTest, maze->maze[iTest][jTest]);
	return 1;
	if( iTest<0 || iTest>=maze->rows || jTest<0 || jTest>=maze->cols || maze->maze[iTest][jTest])
		return 0;
	return 1;
}



void moveAhead(player_t* player, maze_t* maze){
	float rot_x_rad = ( (player->xRot / 180 ) * 3.141592654f);
	float rot_y_rad = ( (player->yRot / 180 )* 3.141592654f);
	
	double varX = sin(rot_y_rad)/player->sensibility;
	double varZ = -cos(rot_y_rad)/player->sensibility;
	double varY = -sin(rot_x_rad)/player->sensibility;
	
	//~ if(player->onFloor){
		//~ player->y = 0.2;
		//~ moveBBox(player->box, 0, 0.2, 0);
	//~ }
	
	bbox_t* tmpX = createBBox(player->box->x1, player->box->y1, player->box->z1,
								player->box->x2, player->box->y2, player->box->z3,
								player->box->x3, player->box->y3, player->box->z3,
								player->box->x4, player->box->y4, player->box->z4,
								player->box->x5, player->box->y5, player->box->z5,
								player->box->x6, player->box->y6, player->box->z6,
								player->box->x7, player->box->y7, player->box->z7,
								player->box->x8, player->box->y8, player->box->z8 );
	bbox_t* tmpZ = createBBox(player->box->x1, player->box->y1, player->box->z1,
								player->box->x2, player->box->y2, player->box->z3,
								player->box->x3, player->box->y3, player->box->z3,
								player->box->x4, player->box->y4, player->box->z4,
								player->box->x5, player->box->y5, player->box->z5,
								player->box->x6, player->box->y6, player->box->z6,
								player->box->x7, player->box->y7, player->box->z7,
								player->box->x8, player->box->y8, player->box->z8 );
	
	moveBBox(tmpX, varX, 0, 0);
	if( hasCollitionBoxList(tmpX, player->bbox_list) ) {
		varX = 0;
	}
	moveBBox(tmpZ, 0, 0, varZ);
	if( hasCollitionBoxList(tmpZ, player->bbox_list) ) {
		varZ = 0;
	}
	free(tmpX);
	free(tmpZ);
	
	player->x += varX;
	player->z += varZ;
	moveBBox(player->box, varX, 0, varZ);
}

void moveBack(player_t* player, maze_t* maze){
	float rot_x_rad = ( (player->xRot / 180 ) * 3.141592654f);
	float rot_y_rad = ( (player->yRot / 180 )* 3.141592654f);
	
	float varX = -sin(rot_y_rad)/player->sensibility;
	float varZ = cos(rot_y_rad)/player->sensibility;
	float varY = -sin(rot_x_rad)/player->sensibility;
	
	bbox_t* tmpX = createBBox(player->box->x1, player->box->y1, player->box->z1,
								player->box->x2, player->box->y2, player->box->z3,
								player->box->x3, player->box->y3, player->box->z3,
								player->box->x4, player->box->y4, player->box->z4,
								player->box->x5, player->box->y5, player->box->z5,
								player->box->x6, player->box->y6, player->box->z6,
								player->box->x7, player->box->y7, player->box->z7,
								player->box->x8, player->box->y8, player->box->z8 );
	bbox_t* tmpZ = createBBox(player->box->x1, player->box->y1, player->box->z1,
								player->box->x2, player->box->y2, player->box->z3,
								player->box->x3, player->box->y3, player->box->z3,
								player->box->x4, player->box->y4, player->box->z4,
								player->box->x5, player->box->y5, player->box->z5,
								player->box->x6, player->box->y6, player->box->z6,
								player->box->x7, player->box->y7, player->box->z7,
								player->box->x8, player->box->y8, player->box->z8 );
	
	moveBBox(tmpX, varX, 0, 0);
	if( hasCollitionBoxList(tmpX, player->bbox_list) ) {
		varX = 0;
	}
	moveBBox(tmpZ, 0, 0, varZ);
	if( hasCollitionBoxList(tmpZ, player->bbox_list) ) {
		varZ = 0;
	}
	free(tmpX);
	free(tmpZ);
	
	player->x += varX;
	player->z += varZ;
	moveBBox(player->box, varX, 0, varZ);
}

void moveLeft(player_t* player, maze_t* maze){
	float rot_x_rad = ( (player->xRot / 180 ) * 3.141592654f);
	float rot_y_rad = ( (player->yRot / 180 )* 3.141592654f);
	
	float varX = -cos(rot_y_rad)/player->sensibility;
	float varZ = -sin(rot_y_rad)/player->sensibility;
	
	bbox_t* tmpX = createBBox(player->box->x1, player->box->y1, player->box->z1,
								player->box->x2, player->box->y2, player->box->z3,
								player->box->x3, player->box->y3, player->box->z3,
								player->box->x4, player->box->y4, player->box->z4,
								player->box->x5, player->box->y5, player->box->z5,
								player->box->x6, player->box->y6, player->box->z6,
								player->box->x7, player->box->y7, player->box->z7,
								player->box->x8, player->box->y8, player->box->z8 );
	bbox_t* tmpZ = createBBox(player->box->x1, player->box->y1, player->box->z1,
								player->box->x2, player->box->y2, player->box->z3,
								player->box->x3, player->box->y3, player->box->z3,
								player->box->x4, player->box->y4, player->box->z4,
								player->box->x5, player->box->y5, player->box->z5,
								player->box->x6, player->box->y6, player->box->z6,
								player->box->x7, player->box->y7, player->box->z7,
								player->box->x8, player->box->y8, player->box->z8 );
	
	moveBBox(tmpX, varX, 0, 0);
	if( hasCollitionBoxList(tmpX, player->bbox_list) ) {
		varX = 0;
	}
	moveBBox(tmpZ, 0, 0, varZ);
	if( hasCollitionBoxList(tmpZ, player->bbox_list) ) {
		varZ = 0;
	}
	free(tmpX);
	free(tmpZ);
	
	player->x += varX;
	player->z += varZ;
	moveBBox(player->box, varX, 0, varZ);
}

void moveRight(player_t* player, maze_t* maze){
	float rot_x_rad = ( (player->xRot / 180 ) * 3.141592654f);
	float rot_y_rad = ( (player->yRot / 180 )* 3.141592654f);
	
	float varX = cos(rot_y_rad)/player->sensibility;
	float varZ = sin(rot_y_rad)/player->sensibility;
	
	bbox_t* tmpX = createBBox(player->box->x1, player->box->y1, player->box->z1,
								player->box->x2, player->box->y2, player->box->z3,
								player->box->x3, player->box->y3, player->box->z3,
								player->box->x4, player->box->y4, player->box->z4,
								player->box->x5, player->box->y5, player->box->z5,
								player->box->x6, player->box->y6, player->box->z6,
								player->box->x7, player->box->y7, player->box->z7,
								player->box->x8, player->box->y8, player->box->z8 );
	bbox_t* tmpZ = createBBox(player->box->x1, player->box->y1, player->box->z1,
								player->box->x2, player->box->y2, player->box->z3,
								player->box->x3, player->box->y3, player->box->z3,
								player->box->x4, player->box->y4, player->box->z4,
								player->box->x5, player->box->y5, player->box->z5,
								player->box->x6, player->box->y6, player->box->z6,
								player->box->x7, player->box->y7, player->box->z7,
								player->box->x8, player->box->y8, player->box->z8 );
	
	moveBBox(tmpX, varX, 0, 0);
	if( hasCollitionBoxList(tmpX, player->bbox_list) ) {
		varX = 0;
	}
	moveBBox(tmpZ, 0, 0, varZ);
	if( hasCollitionBoxList(tmpZ, player->bbox_list) ) {
		varZ = 0;
	}
	free(tmpX);
	free(tmpZ);
	
	player->x += varX;
	player->z += varZ;
	moveBBox(player->box, varX, 0, varZ);
}
