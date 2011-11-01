//      bbox_list.h
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

#ifndef BBOX_LIST_H
#define BBOX_LIST_H

#include <GL/gl.h>
#include "bbox.h"
#include "maze.h"

struct bbox_list_t {
	int size;
	bbox_t** list;
	bbox_t* player_box;
};

typedef struct bbox_list_t bbox_list_t;

bbox_list_t* createBBoxList(maze_t*);

int getBBoxListSize(bbox_list_t*);

void addBBox(bbox_list_t*, bbox_t*);

void deleteBBox(bbox_list_t*, int);

void deleteBBoxList(bbox_list_t*);

bbox_t* getBBox(bbox_list_t* blist, int);

void showBBoxList(bbox_list_t*);

int hasCollitionBoxList(bbox_t*, bbox_list_t*);

#endif


