//      hud.c
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
#include <string.h>
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>

#include "hud.h"
#include "mytimer.h"

hud_t* createHud(maze_t* maze, player_t* player, int w, int h){
	hud_t* hud = malloc(sizeof(hud_t));
	hud->maze = maze;
	hud->player = player;
	hud->curr_time = hud->maze->mytimer->current;
	hud->counter = 0;
	hud->curr_fps = 0;
	hud->fps = malloc(sizeof(char)*5);
	
	return hud;
}

void printMessage(char str[], int x, int y, GLfloat c1, GLfloat c2, GLfloat c3){
	glColor3f(c1, c2, c3);
	int i, length = strlen(str);
	// Position in function of center of string
	glRasterPos2i(x - 12*length/2, y + 12);
	
	for( i=0; i<length; i++ ){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int) str[i]);
	}
}

void checkWin(hud_t* hud){
	if( hud->player->game == DEFAULT && round(hud->player->z) == hud->maze->xf
									&& round(hud->player->x) == hud->maze->yf){
		printf("Hai vinto!\n");
		hud->player->game = WIN;

		FILE *stream;
		char buffer[10];
		char* msg = malloc(sizeof(char)*20);

		stream = popen("ps ax eopid,comm | grep -v grep | grep mplayer | cut -c 1-5", "r");
		while ( fgets(buffer, 10, stream) != NULL ){
			printf("%s", buffer);
			sprintf(msg, "kill -9 %s", buffer);
			system(msg);
			printf("Killed: %s", msg, buffer);
		}
		pclose(stream);
//~ 
		system("mplayer -loop 99999 tada.mp3 &");
	}
	if( hud->player->game == DEFAULT && hud->maze->mytimer->residual == 0 ){
		printf("Hai perso!\n");
		hud->player->game = LOSE;
		
		FILE *stream;
		char buffer[10];
		char* msg = malloc(sizeof(char)*20);

		stream = popen("ps ax eopid,comm | grep -v grep | grep mplayer | cut -c 1-5", "r");
		while ( fgets(buffer, 10, stream) != NULL ){
			printf("%s", buffer);
			sprintf(msg, "kill -9 %s", buffer);
			system(msg);
			printf("Killed: %s", msg, buffer);
		}
		pclose(stream);
//~ 
		system("mplayer -loop 99999 gameover.mp3 &");
	}
}

void updateHud(hud_t* hud, int w, int h){
	updateTimer(hud->maze->mytimer);
	checkWin(hud);
	
	if(hud->maze->mytimer->current == hud->curr_time)
		hud->counter++;
	else{
		hud->curr_fps = hud->counter;
		hud->counter = 1;
		hud->curr_time = hud->maze->mytimer->current;
	}
}

void showOrthoQuad(int x1, int y1, int x2, int y2, GLfloat c1, GLfloat c2, GLfloat c3, GLfloat rot, GLfloat liv){
	glColor4f(c1, c2, c3, 1.0f);
	glPushMatrix();
		glTranslatef(x1 + (x2-x1)/2, y1 + (y2-y1)/2, liv);
		glRotatef(rot, 0, 0, 1);
		x1 = -(x2-x1)/2;
		x2 = -x1;
		y1 = -(y2-y1)/2;
		y2 = -y1;
		glBegin(GL_QUADS);
			glTexCoord2f(0, 1);
			glVertex2f(x1,y1);
			glTexCoord2f(0, 0);
			glVertex2f(x2,y1);
			glTexCoord2f(1, 0);
			glVertex2f(x2,y2);
			glTexCoord2f(1, 1);
			glVertex2f(x1,y2);
		glEnd();
	glPopMatrix();
}

void createCircle(int x, int y, int radius, float c1, float c2, float c3, float liv){
	int i=0;
	float PI = 3.1416;
	glColor3f(c1, c2, c3);
	glBegin(GL_TRIANGLE_FAN);
	for(i=0; i<360; i++){
		glVertex3f(x+radius*cos(i*PI/180), y + radius*sin(i*PI/180), liv);
	}
	glEnd();
}

void drawHud(hud_t* hud, int w, int h){
	
	glDisable(GL_TEXTURE_2D);
	
	double off = h/10;
	int x = w/20, y = h-w/20-off;
	int r = sqrt((off/2)*(off/2)+(off/2)*(off/2));
	int L = 2*r;
	int diff = abs((L - off)/2);
	
	glDisable(GL_LIGHTING);
	createCircle(x + off/2, y + off/2, r, 0, 1, 0, 0);
	createCircle(x + off/2, y + off/2, r + r/8, 0, 0, 1, 0.0);
	
	// Show compass
	glEnable(GL_TEXTURE_2D);
	
	showOrthoQuad(x, y, x + off, y + off, 0.0, 1.0, 0.0, hud->player->yRot, -1);
	
	glDisable(GL_TEXTURE_2D);
	printMessage(getTimer(hud->maze->mytimer), w/4, h/30, 0.0, 0.0, 1.0);
	sprintf(hud->fps, "FPS: %d", hud->curr_fps);
	printMessage(hud->fps, w/2 + w/4, h/30, 0.0, 0.0, 1.0);
	showOrthoQuad(w/20, h/40, w -w/20, 4*h/40, 0.0, 1.0, 0.0, 0.0, 0);
	int offset = h/160;
	showOrthoQuad(w/20 - offset, h/40 - offset, w -w/20 + offset, 4*h/40 + offset, 0.0, 0.0, 1.0, 0.0, 0);
	
	glColor3f(0.0, 0.0, 1.0);
	if(hud->player->game == WIN){
		showOrthoQuad(w/2 - w/6, h/2 - h/10, w/2 + w/6, h/2 + h/6, 0.0, 0.0, 1.0, 0.0, 0.7);
		showOrthoQuad(w/2 - w/7, h/2 - h/14, w/2 + w/7, h/2 + h/8, 0.0, 1.0, 0.0, 0.0, 0.5);
		printMessage("You Win!", w/2, h/2, 1.0, 0.0, 0.0);
	}
	if(hud->player->game == LOSE){
		showOrthoQuad(w/2 - w/6, h/2 - h/10, w/2 + w/6, h/2 + h/6, 0.0, 0.0, 1.0, 0.0, 0.7);
		showOrthoQuad(w/2 - w/7, h/2 - h/14, w/2 + w/7, h/2 + h/8, 0.0, 1.0, 0.0, 0.0, 0.5);
		printMessage("You Lose!", w/2, h/2, 1.0, 0.0, 0.0);
	}
}
