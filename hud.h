//      hud.h
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


#ifndef HUD_H
#define HUD_H

#include <GL/gl.h>
#include "maze.h"
#include "player.h"

struct hud_t {
	maze_t* maze;
	player_t* player;
	GLint counter;
	GLint curr_time;
	GLint curr_fps;
	char* fps;
};

typedef struct hud_t hud_t;

hud_t* createHud(maze_t*, player_t*, int, int);

void updateHud(hud_t*, int, int);

void drawHud(hud_t*, int, int);


#endif


