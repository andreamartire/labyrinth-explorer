#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>

struct texture_t {
	GLubyte *texture;
	GLint width, height, components;
	GLenum format;
};

typedef struct texture_t texture_t;

texture_t* createTexture(char*);

#endif
