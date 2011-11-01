#ifndef BBOX_H
#define BBOX_H

#include <GL/gl.h>

struct bbox_t {
	GLfloat x1, y1, z1;
	GLfloat x2, y2, z2;
	GLfloat x3, y3, z3;
	GLfloat x4, y4, z4;
	GLfloat x5, y5, z5;
	GLfloat x6, y6, z6;
	GLfloat x7, y7, z7;
	GLfloat x8, y8, z8;
};

typedef struct bbox_t bbox_t;

bbox_t* createBBox(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2,
					GLfloat x3, GLfloat y3, GLfloat z3, GLfloat x4, GLfloat y4, GLfloat z4,
					GLfloat x5, GLfloat y5, GLfloat z5, GLfloat x6, GLfloat y6, GLfloat z6,
					GLfloat x7, GLfloat y7, GLfloat z7, GLfloat x8, GLfloat y8, GLfloat z8);

int vertexHasCollition(GLfloat, GLfloat, bbox_t*);

int hasCollition(bbox_t*, bbox_t*);

void moveBBox(bbox_t*, GLfloat, GLfloat, GLfloat);

void showBBox(bbox_t*);

#endif
