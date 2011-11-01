#include "texture.h"
#include <stdio.h>
#include <stdlib.h>

texture_t* createTexture( char *str ){
	texture_t *t = malloc(sizeof(texture_t));
	t->texture = (GLubyte*) gltLoadTGA(str, &t->width, &t->height, &t->components, &t->format);
	return t;
}
