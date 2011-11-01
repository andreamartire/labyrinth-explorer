//      bbox.c
//      
//      Copyright 2011 Andrea Martire <andreamartire@gmail.com>
//      
//      This program is free software; you can redistribute it and/or modify
//      it under the terms of the GNU General Public License as published by
//      the Free Software Foundation; either version 2 of the License, or
//      (at your option) any later version.
//      
//      This program is distributed in the hope that it will be useful,
//      but WITHOUT ANY WARRANTY; without even the implied warranty of
//      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//      GNU General Public License for more details.
//      
//      You should have received a copy of the GNU General Public License
//      along with this program; if not, write to the Free Software
//      Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
//      MA 02110-1301, USA.

#include <stdlib.h>

#include "bbox.h"

bbox_t* createBBox(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2,
					GLfloat x3, GLfloat y3, GLfloat z3, GLfloat x4, GLfloat y4, GLfloat z4,
					GLfloat x5, GLfloat y5, GLfloat z5, GLfloat x6, GLfloat y6, GLfloat z6,
					GLfloat x7, GLfloat y7, GLfloat z7, GLfloat x8, GLfloat y8, GLfloat z8){
	bbox_t* bbox = malloc(sizeof(bbox_t));
	bbox->x1 = x1;bbox->y1 = y1;bbox->z1 = z1;
	bbox->x2 = x2;bbox->y2 = y2;bbox->z2 = z2;
	bbox->x3 = x3;bbox->y3 = y3;bbox->z3 = z3;
	bbox->x4 = x4;bbox->y4 = y4;bbox->z4 = z4;
	bbox->x5 = x5;bbox->y5 = y5;bbox->z5 = z5;
	bbox->x6 = x6;bbox->y6 = y6;bbox->z6 = z6;
	bbox->x7 = x7;bbox->y7 = y7;bbox->z7 = z7;
	bbox->x8 = x8;bbox->y8 = y8;bbox->z8 = z8;
	return bbox;
}

int vertexHasCollition(GLfloat x, GLfloat z, bbox_t* box){
	if( (x > box->x1) && (x < box->x4) && (z > box->z1) && (z < box->z2) )
		return 1;
	return 0;
}

int hasCollition(bbox_t* b1, bbox_t* b2){
	int v1, v2, v3, v4, v5, v6, v7, v8;
	v1 = vertexHasCollition(b1->x1, b1->z1, b2);
	v2 = vertexHasCollition(b1->x2, b1->z2, b2);
	v3 = vertexHasCollition(b1->x3, b1->z3, b2);
	v4 = vertexHasCollition(b1->x4, b1->z4, b2);
	v5 = vertexHasCollition(b1->x5, b1->z5, b2);
	v6 = vertexHasCollition(b1->x6, b1->z6, b2);
	v7 = vertexHasCollition(b1->x7, b1->z7, b2);
	v8 = vertexHasCollition(b1->x8, b1->z8, b2);
	return v1+v2+v3+v4+v5+v6+v7+v8;
}

void moveBBox(bbox_t* box, GLfloat vx, GLfloat vy, GLfloat vz){
	box->x1 += vx; box->y1 += vy; box->z1 += vz;
	box->x2 += vx; box->y2 += vy; box->z2 += vz;
	box->x3 += vx; box->y3 += vy; box->z3 += vz;
	box->x4 += vx; box->y4 += vy; box->z4 += vz;
	box->x5 += vx; box->y5 += vy; box->z5 += vz;
	box->x6 += vx; box->y6 += vy; box->z6 += vz;
	box->x7 += vx; box->y7 += vy; box->z7 += vz;
	box->x8 += vx; box->y8 += vy; box->z8 += vz;
}

void showBBox(bbox_t* bbox){
	glPushMatrix();
		glColor3f(0, 0, 1);
		glBegin(GL_LINE_LOOP);// UP
			glVertex3f(bbox->x1, bbox->y1, bbox->z1);
			glVertex3f(bbox->x2, bbox->y2, bbox->z2);
			glVertex3f(bbox->x3, bbox->y3, bbox->z3);
			glVertex3f(bbox->x4, bbox->y4, bbox->z4);
		glEnd();
		glBegin(GL_LINE_LOOP);// DOWN
			glVertex3f(bbox->x5, bbox->y5, bbox->z5);
			glVertex3f(bbox->x6, bbox->y6, bbox->z6);
			glVertex3f(bbox->x7, bbox->y7, bbox->z7);
			glVertex3f(bbox->x8, bbox->y8, bbox->z8);
		glEnd();
		glBegin(GL_LINE_LOOP);// Main DIAG 
			glVertex3f(bbox->x1, bbox->y1, bbox->z1);
			glVertex3f(bbox->x3, bbox->y3, bbox->z3);
			glVertex3f(bbox->x7, bbox->y7, bbox->z7);
			glVertex3f(bbox->x5, bbox->y5, bbox->z5);
		glEnd();
		glBegin(GL_LINE_LOOP);// Second diag
			glVertex3f(bbox->x2, bbox->y2, bbox->z2);
			glVertex3f(bbox->x4, bbox->y4, bbox->z4);
			glVertex3f(bbox->x8, bbox->y8, bbox->z8);
			glVertex3f(bbox->x6, bbox->y6, bbox->z6);
		glEnd();
		glBegin(GL_LINE_LOOP);// right x
			glVertex3f(bbox->x3, bbox->y3, bbox->z3);
			glVertex3f(bbox->x4, bbox->y4, bbox->z4);
			glVertex3f(bbox->x7, bbox->y7, bbox->z7);
			glVertex3f(bbox->x8, bbox->y8, bbox->z8);
		glEnd();
		glBegin(GL_LINE_LOOP);// left x
			glVertex3f(bbox->x1, bbox->y1, bbox->z1);
			glVertex3f(bbox->x2, bbox->y2, bbox->z2);
			glVertex3f(bbox->x5, bbox->y5, bbox->z5);
			glVertex3f(bbox->x6, bbox->y6, bbox->z6);
		glEnd();
		glBegin(GL_LINE_LOOP);// front x
			glVertex3f(bbox->x2, bbox->y2, bbox->z2);
			glVertex3f(bbox->x3, bbox->y3, bbox->z3);
			glVertex3f(bbox->x6, bbox->y6, bbox->z6);
			glVertex3f(bbox->x7, bbox->y7, bbox->z7);
		glEnd();
		glBegin(GL_LINE_LOOP);// back x
			glVertex3f(bbox->x1, bbox->y1, bbox->z1);
			glVertex3f(bbox->x4, bbox->y4, bbox->z4);
			glVertex3f(bbox->x5, bbox->y5, bbox->z5);
			glVertex3f(bbox->x8, bbox->y8, bbox->z8);
		glEnd();
		glColor3f(1, 1, 1);
	glPopMatrix();
	return;
}
