#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "maze.h"
#include "bbox_list.h"

void drawQuad(float x1, float y1, float z1, float x2, float y2, float z2, 
				float x3, float y3, float z3, float x4, float y4, float z4,
				float tx1, float ty1, float tx2, float ty2, 
				float tx3, float ty3, float tx4, float ty4);

void drawWallBlock();

void drawBoundedWallBlock(bbox_list_t*);

void drawWallBlock_long_x();

void drawWallBlock_long_z();

// turn +x +z
void drawWallBlock_turn_pxpz();

// turn +x -z
void drawWallBlock_turn_pxnz();

// turn -x -z
void drawWallBlock_turn_nxnz();

// turn +x -z
void drawWallBlock_turn_nxpz();

// type wall t positive z -.-
void drawWallBlock_tpz();

// type wall t positive x |-
void drawWallBlock_tpx();

// type wall t negative z -.-
void drawWallBlock_tnz();

// type wall t negative x |-
void drawWallBlock_tnx();

// type wall t all direction -|-
void drawWallBlock_tall();

void drawLabyrinthWall(maze_t*);

void drawBoundedLabyrinthWall(maze_t*, bbox_list_t*);

void drawCompleteLabyrinthWall(maze_t*);

void drawLabyrinthFloor(maze_t*);

int isInLab(int i, int j, int rows, int cols);

void drawLabyrinthOtherFloor(maze_t*);

void drawAxis();

void drawCubeWorld();

void drawLabyrinth(maze_t*, bbox_list_t*);
#endif
