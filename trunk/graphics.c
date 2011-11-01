#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#include "graphics.h"
#include "maze.h"
#include "bbox_list.h"

/* Graphics functions */

void drawQuad(float x1, float y1, float z1, float x2, float y2, float z2, 
				float x3, float y3, float z3, float x4, float y4, float z4,
				float tx1, float ty1, float tx2, float ty2, 
				float tx3, float ty3, float tx4, float ty4 ){
	// t1 0 1
	// t2 0 0
	// t3 1 0
	// t4 1 1
	GLTVector3 v1, v2, v3, normal;
	glBegin(GL_TRIANGLES);
		v1[0] = x1; v1[1] = y1; v1[2] = z1;
		v2[0] = x2; v2[1] = y2; v2[2] = z2;
		v3[0] = x3; v3[1] = y3; v3[2] = z3;
		gltGetNormalVector(v1, v2, v3, normal);
		glNormal3f(normal[0], normal[1], normal[2]);
		glTexCoord2f(tx1, ty1);
		glVertex3f(x1, y1, z1);
		glTexCoord2f(tx2, ty2);
		glVertex3f(x2, y2, z2);
		glTexCoord2f(tx3, ty3);
		glVertex3f(x3, y3, z3);
	glEnd();
	glBegin(GL_TRIANGLES);
		v1[0] = x1; v1[1] = y1; v1[2] = z1;
		v2[0] = x3; v2[1] = y3; v2[2] = z3;
		v3[0] = x4; v3[1] = y4; v3[2] = z4;
		gltGetNormalVector(v1, v2, v3, normal);
		glNormal3f(normal[0], normal[1], normal[2]);
		glTexCoord2f(tx1, ty1);
		glVertex3f(x1, y1, z1);
		glTexCoord2f(tx3, ty3);
		glVertex3f(x3, y3, z3);
		glTexCoord2f(tx4, ty4);
		glVertex3f(x4, y4, z4);
	glEnd();
}

void drawWallBlock(){
	// y positivo
	glPushMatrix();
		glTranslatef(0.0, 1.0, 0.0);
		drawQuad(-0.25, 0.0, -0.25, -0.25, 0.0, 0.25,
				  0.25, 0.0, 0.25, 0.25, 0.0, -0.25,
				  0.0, 0.5, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5);
	glPopMatrix();
	// x positivo
	glPushMatrix();
		glTranslatef(0.25, 0.0, 0.0);
		drawQuad(0.0, 1.0, 0.25, 0.0, 0.0, 0.25,
				 0.0, 0.0, -0.25, 0.0, 1.0, -0.25,
				 0.0, 1.0, 0.0, 0.0, 0.5, 0.0, 0.5, 1.0);
	glPopMatrix();
	// x negativo
	glPushMatrix();
		glTranslatef(-0.25, 0.0, 0.0);
		drawQuad(0.0, 1.0, -0.25, 0.0, 0.0, -0.25,
				 0.0, 0.0, 0.25, 0.0, 1.0, 0.25,
				 0.0, 1.0, 0.0, 0.0, 0.5, 0.0, 0.5, 1.0);
	glPopMatrix();
	// z negativo
	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.25);
		drawQuad(-0.25, 1.0, 0.0, -0.25, 0.0, 0.0,
				  0.25, 0.0, 0.0, 0.25, 1.0, 0.0,
				  0.0, 1.0, 0.0, 0.0, 0.5, 0.0, 0.5, 1.0);
	glPopMatrix();
	// z positivo
	glPushMatrix();
		glTranslatef(0.0, 0.0, -0.25);
		drawQuad(0.25, 1.0, 0.0, 0.25, 0.0, 0.0,
				-0.25, 0.0, 0.0, -0.25, 1.0, 0.0,
				0.0, 1.0, 0.0, 0.0, 0.5, 0.0, 0.5, 1.0);
	glPopMatrix();
	// pavimento interno al cubo
	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.0);
		// y positivo
		drawQuad(-0.25, 0.0, -0.25, -0.25, 0.0, 0.25,
				  0.25, 0.0, 0.25, 0.25, 0.0, -0.25,
				  0.0, 0.5, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5);
		// y negativo
		drawQuad(-0.25, 0.0, 0.25, -0.25, 0.0, -0.25,
				 0.25, 0.0, -0.25, 0.25, 0.0, 0.25,
				 0.0, 0.5, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5);
	glPopMatrix();
}

void drawWallBlock_long_x(int f1, int f2){
	// upper face
	glPushMatrix();
		glTranslatef(0.0, 1.0, 0.0);
		// y positivo
		drawQuad(-0.5, 0.0, -0.25, -0.5, 0.0, 0.25,
				  0.5, 0.0, 0.25, 0.5, 0.0, -0.25,
				  0.0, 0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 0.5);
	glPopMatrix();
	// front face
	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.25);
		glRotatef(-90.0f, 0.0f, 1.0f, 0.0f);
		// x positivo
		drawQuad(0.0, 1.0, 0.5, 0.0, 0.0, 0.5,
				 0.0, 0.0, -0.5, 0.0, 1.0, -0.5,
				 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0);
	glPopMatrix();
	// back face
	glPushMatrix();
		glTranslatef(0.0, 0.0, -0.25);
		glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
		// x positivo
		drawQuad(0.0, 1.0, 0.5, 0.0, 0.0, 0.5,
				 0.0, 0.0, -0.5, 0.0, 1.0, -0.5,
				 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0);
	glPopMatrix();
	if(f1){
		// left face
		glPushMatrix();
			glTranslatef(-0.5, 0.5, 0.0);
			glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
			drawQuad(-0.5, 0.0, -0.25, -0.5, 0.0, 0.25,
					  0.5, 0.0, 0.25, 0.5, 0.0, -0.25,
					  0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0);
		glPopMatrix();
	}
	if(f2){
		// right face
		glPushMatrix();
			glTranslatef(0.5, 0.5, 0.0);
			glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
			drawQuad(-0.5, 0.0, -0.25, -0.5, 0.0, 0.25,
					  0.5, 0.0, 0.25, 0.5, 0.0, -0.25,
					  0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0);
		glPopMatrix();
	}
}

void drawWallBlock_long_z(int f1, int f2){
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		drawWallBlock_long_x(f1,f2);
	glPopMatrix();
}

// turn +x +z
void drawWallBlock_turn_pxpz(){
	glPushMatrix();
		// upper face
		glPushMatrix();
			glTranslatef(0, 1, 0);
			// big part
			drawQuad(-0.25, 0.0, -0.25,-0.25, 0.0, 0.5,
					  0.25, 0.0, 0.5, 0.25, 0.0, -0.25,
					  0.0, 1.0, 0.0, 0.0, 0.5, 0.0, 0.5, 1.0);
			// small part
			drawQuad(0.25, 0.0, -0.25, 0.25, 0.0, 0.25,
					  0.5, 0.0, 0.25, 0.5, 0.0, -0.25,
					  0.0, 0.5, 0.0, 0.0, 0.25, 0.0, 0.25, 0.5);
		glPopMatrix();
		// left face
		glPushMatrix();
			glTranslatef(-0.25, 0, 0);
			drawQuad(0.0, 1.0, -0.25, 0.0, 0.0, -0.25,
					 0.0, 0.0, 0.5, 0.0, 1.0, 0.5,
					 0.0, 1.0, 0.0, 0.0, 0.75, 0.0, 0.75, 1.0);
		glPopMatrix();
		// back face
		glPushMatrix();
			glTranslatef(0.0, 0, -0.25);
			drawQuad(0.5, 1.0, 0.0, 0.5, 0.0, 0.0,
					 -0.25, 0.0, 0.0, -0.25, 1.0, 0.0,
					 0.0, 1.0, 0.0, 0.0, 0.75, 0.0, 0.75, 1.0);
		glPopMatrix();
		// main front face
		//~ glPushMatrix();
			//~ glTranslatef(0.0, 0, 0.5);
			//~ drawQuad(-0.25, 1.0, 0.0, -0.25, 0.0, 0.0,
					 //~ 0.25, 0.0, 0.0, 0.25, 1.0, 0.0,
					 //~ 0.0, 1.0, 0.0, 0.0, 0.5, 0.0, 0.5, 1.0);
		//~ glPopMatrix();
		// second front face
		glPushMatrix();
			glTranslatef(0.0, 0, 0.25);
			drawQuad(0.25, 1.0, 0.0, 0.25, 0.0, 0.0,
					 0.5, 0.0, 0.0, 0.5, 1.0, 0.0,
					 0.75, 1.0, 0.75, 0.0, 1.0, 0.0, 1.0, 1.0);
		glPopMatrix();
		// main right face
		//~ glPushMatrix();
			//~ glTranslatef(0.5, 0, 0.0);
			//~ drawQuad(0.0, 1.0, 0.25, 0.0, 0.0, 0.25,
					 //~ 0.0, 0.0, -0.25, 0.0, 1.0, -0.25,
					 //~ 0.0, 1.0, 0.0, 0.0, 0.5, 0.0, 0.5, 1.0);
		//~ glPopMatrix();
		// second right face
		glPushMatrix();
			glTranslatef(0.25, 0, 0.0);
			drawQuad(0.0, 1.0, 0.5, 0.0, 0.0, 0.5,
					 0.0, 0.0, 0.25, 0.0, 1.0, 0.25,
					 0.0, 1.0, 0.0, 0.0, 0.25, 0.0, 0.25, 1.0);
		glPopMatrix();
	glPopMatrix();
}

// turn +x -z
void drawWallBlock_turn_pxnz(){
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		drawWallBlock_turn_pxpz();
	glPopMatrix();
}

// turn -x -z
void drawWallBlock_turn_nxnz(){
	glPushMatrix();
		glRotatef(180, 0, 1, 0);
		drawWallBlock_turn_pxpz();
	glPopMatrix();
}

// turn +x -z
void drawWallBlock_turn_nxpz(){
	glPushMatrix();
		glRotatef(270, 0, 1, 0);
		drawWallBlock_turn_pxpz();
	glPopMatrix();
}

// type wall t positive z -.-
void drawWallBlock_tpz(){
	glPushMatrix();
		// upper face
		glPushMatrix();
			glTranslatef(0, 1, 0);
			// big part
			drawQuad(-0.25, 0.0, -0.25,-0.25, 0.0, 0.5,
					  0.25, 0.0, 0.5, 0.25, 0.0, -0.25,
					  0.0, 0.5, 0.0, 0.0, 0.5, 0.0, 0.5, 0.5);
			// right small part
			drawQuad(0.25, 0.0, -0.25, 0.25, 0.0, 0.25,
					  0.5, 0.0, 0.25, 0.5, 0.0, -0.25,
					  0.0, 0.5, 0.0, 0.0, 0.25, 0.0, 0.25, 0.5);
			// left small part
			drawQuad(-0.5, 0.0, -0.25, -0.5, 0.0, 0.25,
					  -0.25, 0.0, 0.25, -0.25, 0.0, -0.25,
					  0.0, 0.5, 0.0, 0.0, 0.25, 0.0, 0.25, 0.5);
		glPopMatrix();
		//~ // big left face
		//~ glPushMatrix();
			//~ glTranslatef(-0.5, 0, 0);
			//~ drawQuad(0.0, 1.0, -0.25, 0.0, 0.0, -0.25,
					 //~ 0.0, 0.0, 0.25, 0.0, 1.0, 0.25,
					 //~ 0.0, 1.0, 0.0, 0.0, 0.5, 0.0, 0.5, 1.0);
		//~ glPopMatrix();
		// small left face
		glPushMatrix();
			glTranslatef(-0.25, 0, 0);
			drawQuad(0.0, 1.0, 0.25, 0.0, 0.0, 0.25,
					 0.0, 0.0, 0.5, 0.0, 1.0, 0.5,
					 0.0, 1.0, 0.0, 0.0, 0.25, 0.0, 0.25, 1.0);
		glPopMatrix();
		// back face
		glPushMatrix();
			glTranslatef(0.0, 0, -0.25);
			drawQuad(0.5, 1.0, 0.0, 0.5, 0.0, 0.0,
					 -0.5, 0.0, 0.0, -0.5, 1.0, 0.0,
					 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0);
		glPopMatrix();
		// main front face
		//~ glPushMatrix();
			//~ glTranslatef(0.0, 0, 0.5);
			//~ drawQuad(-0.25, 1.0, 0.0, -0.25, 0.0, 0.0,
					 //~ 0.25, 0.0, 0.0, 0.25, 1.0, 0.0,
					 //~ 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0);
		//~ glPopMatrix();
		// right front face
		glPushMatrix();
			glTranslatef(0.0, 0, 0.25);
			drawQuad(0.25, 1.0, 0.0, 0.25, 0.0, 0.0,
					 0.5, 0.0, 0.0, 0.5, 1.0, 0.0,
					 0.75, 1.0, 0.75, 0.0, 1.0, 0.0, 1.0, 1.0);
		glPopMatrix();
		// left front face
		glPushMatrix();
			glTranslatef(-0.75, 0, 0.25);
			drawQuad(0.25, 1.0, 0.0, 0.25, 0.0, 0.0,
					 0.5, 0.0, 0.0, 0.5, 1.0, 0.0,
					 0.75, 1.0, 0.75, 0.0, 1.0, 0.0, 1.0, 1.0);
		glPopMatrix();
		// main right face
		//~ glPushMatrix();
			//~ glTranslatef(0.5, 0, 0.0);
			//~ drawQuad(0.0, 1.0, 0.25, 0.0, 0.0, 0.25,
					 //~ 0.0, 0.0, -0.25, 0.0, 1.0, -0.25,
					 //~ 0.0, 1.0, 0.0, 0.0, 0.5, 0.0, 0.5, 1.0);
		//~ glPopMatrix();
		// second right face
		glPushMatrix();
			glTranslatef(0.25, 0, 0.0);
			drawQuad(0.0, 1.0, 0.5, 0.0, 0.0, 0.5,
					 0.0, 0.0, 0.25, 0.0, 1.0, 0.25,
					 0.0, 1.0, 0.0, 0.0, 0.25, 0.0, 0.25, 1.0);
		glPopMatrix();
	glPopMatrix();
}

// type wall t positive x |-
void drawWallBlock_tpx(){
	glPushMatrix();
		glRotatef(90, 0, 1, 0);
		drawWallBlock_tpz();
	glPopMatrix();
}

// type wall t negative z -.-
void drawWallBlock_tnz(){
	glPushMatrix();
		glRotatef(180, 0, 1, 0);
		drawWallBlock_tpz();
	glPopMatrix();
}

// type wall t negative x |-
void drawWallBlock_tnx(){
	glPushMatrix();
		glRotatef(270, 0, 1, 0);
		drawWallBlock_tpz();
	glPopMatrix();
}

// type wall t all direction -|-
void drawWallBlock_tall(){
	glPushMatrix();
		drawWallBlock_turn_nxnz();
		drawWallBlock_turn_pxpz();
	glPopMatrix();
}

void drawLabyrinthWall(maze_t* maze){
	int i,j;
	GLint** lab = maze->maze;
	GLint rows = maze->rows;
	GLint cols = maze->cols;
	//~ printf("draw wall\n");
	glPushMatrix();
	for(i=0; i<rows; i++)
		//~ glTranslatef(0.0, 0.0, 0.01);
		for(j=0; j<cols; j++)
			if(lab[i][j]){
				glPushMatrix();
					glTranslatef(j, 0.0, i);
					//~ glTranslatef(0.01 + j*0.01, 0.0, 0.0);
					if( isInLab(i-1, j, rows, cols) && lab[i-1][j] &&
						isInLab(i+1, j, rows, cols) && lab[i+1][j] && 
						isInLab(i, j-1, rows, cols) && lab[i][j-1] &&
						isInLab(i, j+1, rows, cols) && lab[i][j+1])
						drawWallBlock_tall();
					else 
					if( isInLab(i+1, j, rows, cols) && lab[i+1][j] && 
							 isInLab(i, j-1, rows, cols) && lab[i][j-1] &&
							 isInLab(i, j+1, rows, cols) && lab[i][j+1])
							 drawWallBlock_tpz();// -.-
					else if( isInLab(i-1, j, rows, cols) && lab[i-1][j] &&
							 isInLab(i+1, j, rows, cols) && lab[i+1][j] && 
							 isInLab(i, j+1, rows, cols) && lab[i][j+1])
							 drawWallBlock_tpx();// |-
					else if( isInLab(i-1, j, rows, cols) && lab[i-1][j] &&
							 isInLab(i, j-1, rows, cols) && lab[i][j-1] &&
							 isInLab(i, j+1, rows, cols) && lab[i][j+1])
							 drawWallBlock_tnz();// -'-
					else if( isInLab(i-1, j, rows, cols) && lab[i-1][j] &&
							 isInLab(i+1, j, rows, cols) && lab[i+1][j] && 
							 isInLab(i, j-1, rows, cols) && lab[i][j-1] )
							 drawWallBlock_tnx();// -|
					else if( isInLab(i+1, j, rows, cols) && lab[i+1][j] &&
							 isInLab(i, j+1, rows, cols) && lab[i][j+1])
						drawWallBlock_turn_pxpz();// i-
					else if( isInLab(i-1, j, rows, cols) && lab[i-1][j] &&
							 isInLab(i, j+1, rows, cols) && lab[i][j+1])
						drawWallBlock_turn_pxnz();// L
					else if( isInLab(i-1, j, rows, cols) && lab[i-1][j] &&
							 isInLab(i, j-1, rows, cols) && lab[i][j-1])
						drawWallBlock_turn_nxnz();// _I
					else if( isInLab(i+1, j, rows, cols) && lab[i+1][j] &&
							 isInLab(i, j-1, rows, cols) && lab[i][j-1])
						drawWallBlock_turn_nxpz();// -i
					else 
					if( (isInLab(i-1, j, rows, cols) && lab[i-1][j]) &&
							 (isInLab(i+1, j, rows, cols) && lab[i+1][j]) )
						drawWallBlock_long_z(0,0);// | 
					else
					if( (isInLab(i-1, j, rows, cols) && lab[i-1][j]) &&
							 !isInLab(i+1, j, rows, cols) )
						drawWallBlock_long_z(1,0);// | empty near border pz
					else
					if( (isInLab(i+1, j, rows, cols) && lab[i+1][j]) &&
							 !isInLab(i-1, j, rows, cols) )
						drawWallBlock_long_z(0,1);// | empty near border nz
					else
					if( (isInLab(i-1, j, rows, cols) && lab[i-1][j]) &&
							 (isInLab(i+1, j, rows, cols) && !lab[i+1][j]) )
						drawWallBlock_long_z(1,0);// | to n z
					else
					if( (isInLab(i-1, j, rows, cols) && !lab[i-1][j]) &&
							 (isInLab(i+1, j, rows, cols) && lab[i+1][j]) )
						drawWallBlock_long_z(0,1);// | to p z
					else if( (isInLab(i, j-1, rows, cols) && lab[i][j-1]) &&
							 (isInLab(i, j+1, rows, cols) && lab[i][j+1]) )
						drawWallBlock_long_x(0,0);// --
					else if( (isInLab(i, j-1, rows, cols) && lab[i][j-1]) &&
							 (!isInLab(i, j+1, rows, cols)) )
						drawWallBlock_long_x(0,1);// -- empty near border px
					else if( (isInLab(i, j+1, rows, cols) && lab[i][j+1]) &&
							 (!isInLab(i, j-1, rows, cols)) )
						drawWallBlock_long_x(1,0);// -- empty near border nx
					else if( (isInLab(i, j-1, rows, cols) && lab[i][j-1]) &&
							 (isInLab(i, j+1, rows, cols) && !lab[i][j+1]) )
						drawWallBlock_long_x(0,1);// -- to n x
					else if( (isInLab(i, j-1, rows, cols) && !lab[i][j-1]) &&
							 (isInLab(i, j+1, rows, cols) && lab[i][j+1]) )
						drawWallBlock_long_x(1,0);// -- to p x
					else
						drawWallBlock();
				
				glPopMatrix();
			}
		glPopMatrix();
}

void drawBoundedLabyrinthWall(maze_t* maze, bbox_list_t* bbox_list){
	showBBoxList(bbox_list);
}

void drawCompleteLabyrinthWall(maze_t* maze){
	drawLabyrinthWall(maze);
	glPushMatrix();
		glTranslatef(0.0, 1.0, 0.0);
		drawLabyrinthWall(maze);
	glPopMatrix();
}

int*** crazyInfo;
int*** crazyInfo2;
int okCrazy = 0;

int*** generateCrazyInfo(int** lab, int rows, int cols){
	int i,j;
	//~ printf("draw fantasy wall\n");
	int*** fantasyInfo = malloc(sizeof(int**)*rows);
	for(i=0; i<rows; i++){
		fantasyInfo[i] = malloc(sizeof(int*)*cols);
		for(j=0; j<cols; j++){
			//~ printf("i=%d j=%d ", i, j);
			fantasyInfo[i][j] = malloc(sizeof(int)*4);
			if(lab[i][j]){
				//~ fantasyInfo[i][j][0] = (float)(((float)(rand() % 1001))/1000.0f);
				fantasyInfo[i][j][0] = rand() % 2;
				//~ printf("\t val=%d", fantasyInfo[i][j][0]);
				fantasyInfo[i][j][1] = rand() % 2;
				//~ printf("\t val=%d", fantasyInfo[i][j][1]);
				fantasyInfo[i][j][2] = rand() % 2;
				//~ printf("\t val=%d", fantasyInfo[i][j][2]);
				fantasyInfo[i][j][3] = rand() % 3;
				//~ printf("\t val=%d", fantasyInfo[i][j][3]);
			}
			else{
				fantasyInfo[i][j][0] = fantasyInfo[i][j][1] = fantasyInfo[i][j][2] = fantasyInfo[i][j][3] = -1;
			}
			//~ printf("\n");
		}
	}
	//~ printf("generated\n");
	return fantasyInfo;
}

void drawFantasyLabyrinthWall(maze_t* maze){
	int i,j;
	int rr, rg, rb, rt;
	int** lab = maze->maze;
	int rows = maze->rows;
	int cols = maze->cols;
	//~ printf("draw fantasy wall\n");
	if(!okCrazy){
		okCrazy = 1;
		srand(time(0));	
		crazyInfo = generateCrazyInfo(lab, rows, cols);
		crazyInfo2 = generateCrazyInfo(lab, rows, cols);
	}
	for(i=0; i<rows; i++)
		for(j=0; j<cols; j++){
			//~ printf("i=%d j=%d\n", i, j);
			if(lab[i][j]){
				glPushMatrix();
					glTranslatef(j, 0.0, i);
					
					//~ printf("provo i=%d j=%d\n", i, j);
					rr = crazyInfo[i][j][0];
					rg = crazyInfo[i][j][1];
					rb = crazyInfo[i][j][2];
					rt = crazyInfo[i][j][3];
					glColor3f(rr, rg, rb);
					switch (rt) {
						case 0:
							glPushMatrix();
								glRotatef(90, -1, 0, 0);
								glutSolidCone(0.5f, 1.0, 10, 10);
							glPopMatrix();break;
						case 1:
							glPushMatrix();
								glTranslatef(0.0, 0.5, 0.0);
								glutSolidCube(1.0);
							glPopMatrix();break;
						case 2:
							glPushMatrix();
								glTranslatef(0.0, 0.5, 0.0);
								glutSolidSphere(0.5f, 20, 10);
							glPopMatrix();break;
					};
				glPopMatrix();
				glPushMatrix();
					glTranslatef(j, 1.0, i);
						rr = crazyInfo2[i][j][0];
						rg = crazyInfo2[i][j][1];
						rb = crazyInfo2[i][j][2];
						rt = crazyInfo2[i][j][3];
						glColor3f(rr, rg, rb);
						switch (rt) {
							case 0:
								glPushMatrix();
									glRotatef(90, -1, 0, 0);
									glutSolidCone(0.5f, 1.0, 10, 10);
								glPopMatrix();break;
							case 1:
								glPushMatrix();
									glTranslatef(0.0, 0.5, 0.0);
									glutSolidCube(1.0);
								glPopMatrix();break;
							case 2:
								glPushMatrix();
									glTranslatef(0.0, 0.5, 0.0);
									glutSolidSphere(0.5f, 20, 10);
								glPopMatrix();break;
						};
					glPopMatrix();
				}
			}
	glColor3f(1.0f, 1.0f, 1.0f);
	//~ printf("fine\n");
}

void drawLabyrinthFloor(maze_t* maze){
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	int i,j;
	//~ printf("draw floor\n");
	for(i=0; i<maze->rows; i++)
		for(j=0; j<maze->cols; j++)
			//~ if(!lab[i][j])
			{
				glPushMatrix();
					glTranslatef(j,0,i);
					// y positivo
					drawQuad(-0.5, 0.0, -0.5, -0.5, 0.0, 0.5,
							  0.5, 0.0, 0.5, 0.5, 0.0, -0.5,
							  0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0);
					// y negativo
					drawQuad(-0.5, 0.0, 0.5, -0.5, 0.0, -0.5,
							 0.5, 0.0, -0.5, 0.5, 0.0, 0.5,
							 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0);
				glPopMatrix();
			}
}

int isInLab(int i, int j, int rows, int cols){
	if( i>=0 && i<rows && j>=0 && j<cols)
		return 1;
	return 0;
}

void drawLabyrinthOtherFloor(maze_t* maze){
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	int i,j;
	//~ printf("draw floor\n");
	for(i=-50; i<50; i++)
		for(j=-50; j<50; j++)
			if(!isInLab(i,j,maze->rows,maze->cols)){
				glPushMatrix();
					glTranslatef(j,0,i);
					// y positivo
					drawQuad(-0.5, 0.0, -0.5, -0.5, 0.0, 0.5,
							  0.5, 0.0, 0.5, 0.5, 0.0, -0.5,
							  0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0);
					// y negativo
					drawQuad(-0.5, 0.0, 0.5, -0.5, 0.0, -0.5,
							 0.5, 0.0, -0.5, 0.5, 0.0, 0.5,
							 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 1.0, 1.0);
				glPopMatrix();
			}
}

void drawAxis(){
	// Draw axis X
	glColor3f(1.0f, 0.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(50.0, 0.0, 0.0);
	glEnd();
	
	// Draw axis Y
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 50.0, 0.0);
	glEnd();
	
	// Draw axis Z
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 50.0);
	glEnd();
}

void drawCubeWorld(int r, int c, int alpha){
	int m;
	if(r > c)
		m = r;
	else
		m = c;
	m = 2*m;
	glColor3f(0.7,0.7,1);
	//~ glDisable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glutSolidSphere(2*m,100,100);
	glFrontFace(GL_CCW);
	//~ glEnable(GL_CULL_FACE);

	//~ glPushMatrix();
		//~ glTranslatef(0, -2*m, 0);
		//~ drawQuad(-2*m, 0, -2*m, -2*m, 0, 2*m, 2*m, 0, 2*m, 2*m, 0, -2*m,
				//~ 1, 0, 0, 0, 0, 1, 1, 1);
	//~ glPopMatrix();
	//~ glPushMatrix();
		//~ glTranslatef(0, 2*m, 0);
		//~ glRotatef(180, 0, 0, 1);
		//~ drawQuad(-2*m, 0, -2*m, -2*m, 0, 2*m, 2*m, 0, 2*m, 2*m, 0, -2*m,
				//~ 1, 0, 0, 0, 0, 1, 1, 1);
	//~ glPopMatrix();
	//~ glPushMatrix();
		//~ glTranslatef(2*m, 0, 0);
		//~ glRotatef(90, 0, 0, 1);
		//~ drawQuad(-2*m, 0, -2*m, -2*m, 0, 2*m, 2*m, 0, 2*m, 2*m, 0, -2*m,
				//~ 1, 0, 0, 0, 0, 1, 1, 1);
	//~ glPopMatrix();
	//~ glPushMatrix();
		//~ glTranslatef(-2*m, 0, 0);
		//~ glRotatef(-90, 0, 0, 1);
		//~ drawQuad(-2*m, 0, -2*m, -2*m, 0, 2*m, 2*m, 0, 2*m, 2*m, 0, -2*m,
				//~ 1, 0, 0, 0, 0, 1, 1, 1);
	//~ glPopMatrix();
	//~ glPushMatrix();
		//~ glTranslatef(0, 0, 2*m);
		//~ glRotatef(-90, 1, 0, 0);
		//~ drawQuad(-2*m, 0, -2*m, -2*m, 0, 2*m, 2*m, 0, 2*m, 2*m, 0, -2*m,
				//~ 1, 0, 0, 0, 0, 1, 1, 1);
	//~ glPopMatrix();
	//~ glPushMatrix();
		//~ glTranslatef(0, 0, -2*m);
		//~ glRotatef(90, 1, 0, 0);
		//~ drawQuad(-2*m, 0, -2*m, -2*m, 0, 2*m, 2*m, 0, 2*m, 2*m, 0, -2*m,
				//~ 1, 0, 0, 0, 0, 1, 1, 1);
	//~ glPopMatrix();
	glPushMatrix();
		glRotatef(alpha,0,0,1);
		glTranslatef(m,-m/5,0);
		glColor3f(1,0,0);
		glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
	glPushMatrix();
		glRotatef(alpha,0,1,1);
		glTranslatef(-m-1,m/4,0);
		glColor3f(0.25,1,0);
		glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
	glPushMatrix();
		glRotatef(alpha,0,1,0.5);
		glTranslatef(-m-2,2,0);
		glColor3f(1,1,0);
		glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
	glPushMatrix();
		glRotatef(-alpha,0,0.5,0.5);
		glTranslatef(m+3,m/3,0);
		glColor3f(1,0.25,0.25);
		glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
	glPushMatrix();
		glRotatef(-alpha,1,1,1);
		glTranslatef(m+4,m,0);
		glColor3f(0.5,0.5,0);
		glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
	glPushMatrix();
		glRotatef(-alpha,1,0,1);
		glTranslatef(m+5,m/2,0);
		glColor3f(0.5,1,0);
		glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
	glPushMatrix();
		glRotatef(-alpha,0.5,0.5,0);
		glTranslatef(-m-6,0,0);
		glColor3f(1,1,0.5);
		glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
	glPushMatrix();
		glRotatef(-alpha,0.5,1,0);
		glTranslatef(m+7,0,0);
		glColor3f(0,0,1);
		glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
	glPushMatrix();
		glRotatef(-alpha,1,0.25,0.75);
		glTranslatef(-m-8,0,0);
		glColor3f(0,1,0);
		glutSolidSphere(0.5, 10, 10);
	glPopMatrix();
	glColor3f(1,1,1);
}

void drawLabyrinth(maze_t* maze, bbox_list_t* bbox_list){
	if( maze->oldtype != maze->type ){
		deleteBBoxList(bbox_list);
		bbox_list_t* tmp = createBBoxList(maze);
		bbox_list->size = tmp->size;
		bbox_list->list = tmp->list;
		maze->oldtype = maze->type;
	}
	if( maze->type == STANDARD ){
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		drawCompleteLabyrinthWall(maze);
	}
	else if( maze->type == CRAZY ){
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		drawFantasyLabyrinthWall(maze);
	}
	if( maze->bounded )
		drawBoundedLabyrinthWall(maze, bbox_list);
}
