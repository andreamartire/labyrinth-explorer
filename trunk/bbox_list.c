//      bbox_list.c
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
#include <stdio.h>

#include "bbox_list.h"
#include "bbox.h"
#include "maze.h"

int getBBoxListSize(bbox_list_t* list){
	return list->size;
}

void addBBox(bbox_list_t* blist, bbox_t* bbox){
	if( blist->size == 0 ){
		blist->list = malloc(sizeof(bbox_t*));
		blist->list[0] = bbox;
		//~ printf("Added in position %d\n", blist->size);
		blist->size++;
	}
	else{
		bbox_t** old_bbox_list = blist->list;
		blist->list = malloc(sizeof(bbox_t*)*(blist->size+1));
		int i;
		for(i=0; i<blist->size; i++){
			blist->list[i] = old_bbox_list[i];
		}
		blist->list[blist->size] = bbox;
		//~ printf("Added in position %d\n", blist->size);
		blist->size++;
	}
	return;
}

void deleteBBox(bbox_list_t* blist, int j){
	free(blist->list[j]);
	if(blist->size-1 > 0 ){
		bbox_t** old_bbox_list = blist->list;
		blist->list = malloc(sizeof(bbox_t*)*(blist->size-1));
		int i, k=0;
		for(i=0; i<blist->size; i++)
			if(i!=j){
				blist->list[k++] = old_bbox_list[i];
			}
	}
	blist->size--;
	return;
}

void deleteBBoxList(bbox_list_t* bbox_list){
	int i;
	for( i=0; i<bbox_list->size; i++ ){
		deleteBBox(bbox_list, i);
	}
}

bbox_t* getBBox(bbox_list_t* blist, int i){
	if( i < blist->size )
		return blist->list[i];
	printf("Error: index error\n");
	return NULL;
}

void showBBoxList(bbox_list_t* blist){
	int i;
	for(i=0; i<blist->size; i++)
		showBBox(blist->list[i]);
	showBBox(blist->player_box);
}

void createBoundedWallBlock(bbox_list_t* bbox_list, int i, int j, int h){
	glPushMatrix();
		bbox_t* bbox = createBBox(j-0.5, 0.0, i-0.5, j-0.5, 0.0, i+0.5, j+0.5, 0.0, i+0.5, j+0.5, 0.0, i-0.5, 
									j-0.5, 1.0, i-0.5, j-0.5, 1.0, i+0.5, j+0.5, 1.0, i+0.5, j+0.5, 1.0, i-0.5);
		addBBox(bbox_list, bbox);
		if(h>0){
			bbox_t* bbox2 = createBBox(j-0.5, h, i-0.5, j-0.5, h, i+0.5, j+0.5, h, i+0.5, j+0.5, h, i-0.5, 
									j-0.5, h+1.0, i-0.5, j-0.5, h+1.0, i+0.5, j+0.5, h+1.0, i+0.5, j+0.5, h+1.0, i-0.5);
			addBBox(bbox_list, bbox2);
		}		
	glPopMatrix();
}

void createBounded_turn_pxpz(bbox_list_t* bbox_list, int i, int j, int h){
	glPushMatrix();
		bbox_t* bbox_p1 = createBBox(j-0.25, 0.0, i-0.25, j-0.25, 0.0, i+0.5,
									j+0.25, 0.0, i+0.5, j+0.25, 0.0, i-0.25, 
									j-0.25, 1.0, i-0.25, j-0.25, 1.0, i+0.5,
									j+0.25, 1.0, i+0.5, j+0.25, 1.0, i-0.25);
		addBBox(bbox_list, bbox_p1);
		bbox_t* bbox_p2 = createBBox(j+0.25, 0.0, i-0.25, j+0.25, 0.0, i+0.25,
									j+0.5, 0.0, i+0.25, j+0.5, 0.0, i-0.25, 
									j+0.25, 1.0, i-0.25, j+0.25, 1.0, i+0.25,
									j+0.5, 1.0, i+0.25, j+0.5, 1.0, i-0.25);
		addBBox(bbox_list, bbox_p2);
		if(h>0){
			bbox_t* bbox_p1 = createBBox(j-0.25, h+0.0, i-0.25, j-0.25, h+0.0, i+0.5,
									j+0.25, h+0.0, i+0.5, j+0.25, h+0.0, i-0.25, 
									j-0.25, h+1.0, i-0.25, j-0.25, h+1.0, i+0.5,
									j+0.25, h+1.0, i+0.5, j+0.25, h+1.0, i-0.25);
			addBBox(bbox_list, bbox_p1);
			bbox_t* bbox_p2 = createBBox(j+0.25, h+0.0, i-0.25, j+0.25, h+0.0, i+0.25,
										j+0.5, h+0.0, i+0.25, j+0.5, h+0.0, i-0.25, 
										j+0.25, h+1.0, i-0.25, j+0.25, h+1.0, i+0.25,
										j+0.5, h+1.0, i+0.25, j+0.5, h+1.0, i-0.25);
			addBBox(bbox_list, bbox_p2);
		}
	glPopMatrix();
}

void createBounded_turn_pxnz(bbox_list_t* bbox_list, int i, int j, int h){
	glPushMatrix();
		bbox_t* bbox_p1 = createBBox(j-0.25, 0.0, i-0.5, j-0.25, 0.0, i+0.25,
									j+0.25, 0.0, i+0.25, j+0.25, 0.0, i-0.5, 
									j-0.25, 1.0, i-0.5, j-0.25, 1.0, i+0.25,
									j+0.25, 1.0, i+0.25, j+0.25, 1.0, i-0.5);
		addBBox(bbox_list, bbox_p1);
		bbox_t* bbox_p2 = createBBox(j+0.25, 0.0, i-0.25, j+0.25, 0.0, i+0.25,
									j+0.5, 0.0, i+0.25, j+0.5, 0.0, i-0.25, 
									j+0.25, 1.0, i-0.25, j+0.25, 1.0, i+0.25,
									j+0.5, 1.0, i+0.25, j+0.5, 1.0, i-0.25);
		addBBox(bbox_list, bbox_p2);
		if(h>0){
			bbox_t* bbox_p1 = createBBox(j-0.25, h+0.0, i-0.5, j-0.25, h+0.0, i+0.25,
									j+0.25, h+0.0, i+0.25, j+0.25, h+0.0, i-0.5, 
									j-0.25, h+1.0, i-0.5, j-0.25, h+1.0, i+0.25,
									j+0.25, h+1.0, i+0.25, j+0.25, h+1.0, i-0.5);
			addBBox(bbox_list, bbox_p1);
			bbox_t* bbox_p2 = createBBox(j+0.25, h+0.0, i-0.25, j+0.25, h+0.0, i+0.25,
										j+0.5, h+0.0, i+0.25, j+0.5, h+0.0, i-0.25, 
										j+0.25, h+1.0, i-0.25, j+0.25, h+1.0, i+0.25,
										j+0.5, h+1.0, i+0.25, j+0.5, h+1.0, i-0.25);
			addBBox(bbox_list, bbox_p2);
		}
	glPopMatrix();
}

void createBounded_turn_nxnz(bbox_list_t* bbox_list, int i, int j, int h){
	glPushMatrix();
		bbox_t* bbox_p1 = createBBox(j-0.25, 0.0, i-0.5, j-0.25, 0.0, i+0.25,
									j+0.25, 0.0, i+0.25, j+0.25, 0.0, i-0.5, 
									j-0.25, 1.0, i-0.5, j-0.25, 1.0, i+0.25,
									j+0.25, 1.0, i+0.25, j+0.25, 1.0, i-0.5);
		addBBox(bbox_list, bbox_p1);
		bbox_t* bbox_p2 = createBBox(j-0.5, 0.0, i-0.25, j-0.5, 0.0, i+0.25,
									j-0.25, 0.0, i+0.25, j-0.25, 0.0, i-0.25, 
									j-0.5, 1.0, i-0.25, j-0.5, 1.0, i+0.25,
									j-0.25, 1.0, i+0.25, j-0.25, 1.0, i-0.255);
		addBBox(bbox_list, bbox_p2);
		if(h>0){
			bbox_t* bbox_p1 = createBBox(j-0.25, h+0.0, i-0.5, j-0.25, h+0.0, i+0.25,
									j+0.25, h+0.0, i+0.25, j+0.25, h+0.0, i-0.5, 
									j-0.25, h+1.0, i-0.5, j-0.25, h+1.0, i+0.25,
									j+0.25, h+1.0, i+0.25, j+0.25, h+1.0, i-0.5);
			addBBox(bbox_list, bbox_p1);
			bbox_t* bbox_p2 = createBBox(j-0.5, h+0.0, i-0.25, j-0.5, h+0.0, i+0.25,
									j-0.25, h+0.0, i+0.25, j-0.25, h+0.0, i-0.25, 
									j-0.5, h+1.0, i-0.25, j-0.5, h+1.0, i+0.25,
									j-0.25, h+1.0, i+0.25, j-0.25, h+1.0, i-0.255);
			addBBox(bbox_list, bbox_p2);
		}
	glPopMatrix();
}

void createBounded_turn_nxpz(bbox_list_t* bbox_list, int i, int j, int h){
	glPushMatrix();
		bbox_t* bbox_p1 = createBBox(j-0.25, 0.0, i-0.25, j-0.25, 0.0, i+0.5,
									j+0.25, 0.0, i+0.5, j+0.25, 0.0, i-0.25, 
									j-0.25, 1.0, i-0.25, j-0.25, 1.0, i+0.5,
									j+0.25, 1.0, i+0.5, j+0.25, 1.0, i-0.25);
		addBBox(bbox_list, bbox_p1);
		bbox_t* bbox_p2 = createBBox(j-0.5, 0.0, i-0.25, j-0.5, 0.0, i+0.25,
									j-0.25, 0.0, i+0.25, j-0.25, 0.0, i-0.25, 
									j-0.5, 1.0, i-0.25, j-0.5, 1.0, i+0.25,
									j-0.25, 1.0, i+0.25, j-0.25, 1.0, i-0.255);
		addBBox(bbox_list, bbox_p2);
		if(h>0){
			bbox_t* bbox_p1 = createBBox(j-0.25, h+0.0, i-0.25, j-0.25, h+0.0, i+0.5,
									j+0.25, h+0.0, i+0.5, j+0.25, h+0.0, i-0.25, 
									j-0.25, h+1.0, i-0.25, j-0.25, h+1.0, i+0.5,
									j+0.25, h+1.0, i+0.5, j+0.25, h+1.0, i-0.25);
			addBBox(bbox_list, bbox_p1);
			bbox_t* bbox_p2 = createBBox(j-0.5, h+0.0, i-0.25, j-0.5, h+0.0, i+0.25,
									j-0.25, h+0.0, i+0.25, j-0.25, h+0.0, i-0.25, 
									j-0.5, h+1.0, i-0.25, j-0.5, h+1.0, i+0.25,
									j-0.25, h+1.0, i+0.25, j-0.25, h+1.0, i-0.255);
			addBBox(bbox_list, bbox_p2);
		}
	glPopMatrix();
}

void createBounded_tpx(bbox_list_t* bbox_list, int i, int j, int h){
	glPushMatrix();
		bbox_t* bbox_p1 = createBBox(j-0.25, 0.0, i-0.5, j-0.25, 0.0, i+0.5,
									j+0.25, 0.0, i+0.5, j+0.25, 0.0, i-0.5, 
									j-0.25, 1.0, i-0.5, j-0.25, 1.0, i+0.5,
									j+0.25, 1.0, i+0.5, j+0.25, 1.0, i-0.5);
		addBBox(bbox_list, bbox_p1);
		bbox_t* bbox_p2 = createBBox(j+0.25, 0.0, i-0.25, j+0.25, 0.0, i+0.25,
									j+0.5, 0.0, i+0.25, j+0.5, 0.0, i-0.25, 
									j+0.25, 1.0, i-0.25, j+0.25, 1.0, i+0.25,
									j+0.5, 1.0, i+0.25, j+0.5, 1.0, i-0.25);
		addBBox(bbox_list, bbox_p2);
		if(h>0){
			bbox_t* bbox_p1 = createBBox(j-0.25, h+0.0, i-0.5, j-0.25, h+0.0, i+0.5,
									j+0.25, h+0.0, i+0.5, j+0.25, h+0.0, i-0.5, 
									j-0.25, h+1.0, i-0.5, j-0.25, h+1.0, i+0.5,
									j+0.25, h+1.0, i+0.5, j+0.25, h+1.0, i-0.5);
			addBBox(bbox_list, bbox_p1);
			bbox_t* bbox_p2 = createBBox(j+0.25, h+0.0, i-0.25, j+0.25, h+0.0, i+0.25,
										j+0.5, h+0.0, i+0.25, j+0.5, h+0.0, i-0.25, 
										j+0.25, h+1.0, i-0.25, j+0.25, h+1.0, i+0.25,
										j+0.5, h+1.0, i+0.25, j+0.5, h+1.0, i-0.25);
			addBBox(bbox_list, bbox_p2);
		}
	glPopMatrix();
}

void createBounded_tnx(bbox_list_t* bbox_list, int i, int j, int h){
	glPushMatrix();
		bbox_t* bbox_p1 = createBBox(j-0.25, 0.0, i-0.5, j-0.25, 0.0, i+0.5,
									j+0.25, 0.0, i+0.5, j+0.25, 0.0, i-0.5, 
									j-0.25, 1.0, i-0.5, j-0.25, 1.0, i+0.5,
									j+0.25, 1.0, i+0.5, j+0.25, 1.0, i-0.5);
		addBBox(bbox_list, bbox_p1);
		bbox_t* bbox_p2 = createBBox(j-0.5, 0.0, i-0.25, j-0.5, 0.0, i+0.25,
									j-0.25, 0.0, i+0.25, j-0.25, 0.0, i-0.25, 
									j-0.5, 1.0, i-0.25, j-0.5, 1.0, i+0.25,
									j-0.25, 1.0, i+0.25, j-0.25, 1.0, i-0.255);
		addBBox(bbox_list, bbox_p2);
		if(h>0){
			bbox_t* bbox_p1 = createBBox(j-0.25, h+0.0, i-0.5, j-0.25, h+0.0, i+0.5,
									j+0.25, h+0.0, i+0.5, j+0.25, h+0.0, i-0.5, 
									j-0.25, h+1.0, i-0.5, j-0.25, h+1.0, i+0.5,
									j+0.25, h+1.0, i+0.5, j+0.25, h+1.0, i-0.5);
			addBBox(bbox_list, bbox_p1);
			bbox_t* bbox_p2 = createBBox(j-0.5, h+0.0, i-0.25, j-0.5, h+0.0, i+0.25,
									j-0.25, h+0.0, i+0.25, j-0.25, h+0.0, i-0.25, 
									j-0.5, h+1.0, i-0.25, j-0.5, h+1.0, i+0.25,
									j-0.25, h+1.0, i+0.25, j-0.25, h+1.0, i-0.255);
			addBBox(bbox_list, bbox_p2);
		}
	glPopMatrix();
}

void createBounded_tpz(bbox_list_t* bbox_list, int i, int j, int h){
	glPushMatrix();
		bbox_t* bbox_p1 = createBBox(j-0.5, 0.0, i-0.25, j-0.5, 0.0, i+0.25,
									j+0.5, 0.0, i+0.25, j+0.5, 0.0, i-0.25, 
									j-0.5, 1.0, i-0.25, j-0.5, 1.0, i+0.25,
									j+0.5, 1.0, i+0.25, j+0.5, 1.0, i-0.25);
		addBBox(bbox_list, bbox_p1);
		bbox_t* bbox_p2 = createBBox(j-0.25, 0.0, i+0.25, j-0.25, 0.0, i+0.5,
									j+0.25, 0.0, i+0.5, j+0.25, 0.0, i+0.25, 
									j-0.25, 1.0, i+0.25, j-0.25, 1.0, i+0.5,
									j+0.25, 1.0, i+0.5, j+0.25, 1.0, i+0.25);
		addBBox(bbox_list, bbox_p2);
		if(h>0){
			bbox_t* bbox_p1 = createBBox(j-0.5, 0.0, i-0.25, j-0.5, 0.0, i+0.25,
									j+0.5, 0.0, i+0.25, j+0.5, 0.0, i-0.25, 
									j-0.5, h+1.0, i-0.25, j-0.5, h+1.0, i+0.25,
									j+0.5, h+1.0, i+0.25, j+0.5, h+1.0, i-0.25);
			addBBox(bbox_list, bbox_p1);
			bbox_t* bbox_p2 = createBBox(j-0.25, h+0.0, i+0.25, j-0.25, h+0.0, i+0.5,
									j+0.25, h+0.0, i+0.5, j+0.25, h+0.0, i+0.25, 
									j-0.25, h+1.0, i+0.25, j-0.25, h+1.0, i+0.5,
									j+0.25, h+1.0, i+0.5, j+0.25, h+1.0, i+0.25);
			addBBox(bbox_list, bbox_p2);
		}
	glPopMatrix();
}

void createBounded_tnz(bbox_list_t* bbox_list, int i, int j, int h){
	glPushMatrix();
		bbox_t* bbox_p1 = createBBox(j-0.5, 0.0, i-0.25, j-0.5, 0.0, i+0.25,
									j+0.5, 0.0, i+0.25, j+0.5, 0.0, i-0.25, 
									j-0.5, 1.0, i-0.25, j-0.5, 1.0, i+0.25,
									j+0.5, 1.0, i+0.25, j+0.5, 1.0, i-0.25);
		addBBox(bbox_list, bbox_p1);
		bbox_t* bbox_p2 = createBBox(j-0.25, 0.0, i-0.5, j-0.25, 0.0, i-0.25,
									j+0.25, 0.0, i-0.25, j+0.25, 0.0, i-0.5, 
									j-0.25, 1.0, i-0.5, j-0.25, 1.0, i-0.25,
									j+0.25, 1.0, i-0.25, j+0.25, 1.0, i-0.5);
		addBBox(bbox_list, bbox_p2);
		if(h>0){
			bbox_t* bbox_p1 = createBBox(j-0.5, 0.0, i-0.25, j-0.5, 0.0, i+0.25,
									j+0.5, 0.0, i+0.25, j+0.5, 0.0, i-0.25, 
									j-0.5, h+1.0, i-0.25, j-0.5, h+1.0, i+0.25,
									j+0.5, h+1.0, i+0.25, j+0.5, h+1.0, i-0.25);
			addBBox(bbox_list, bbox_p1);
			bbox_t* bbox_p2 = createBBox(j-0.25, h+0.0, i-0.5, j-0.25, h+0.0, i-0.25,
									j+0.25, h+0.0, i-0.25, j+0.25, h+0.0, i-0.5, 
									j-0.25, h+1.0, i-0.5, j-0.25, h+1.0, i-0.25,
									j+0.25, h+1.0, i-0.25, j+0.25, h+1.0, i-0.5);
			addBBox(bbox_list, bbox_p2);
		}
	glPopMatrix();
}

void createBounded_longx(bbox_list_t* bbox_list, int i, int j, int h){
	glPushMatrix();
		bbox_t* bbox_p1 = createBBox(j-0.5, 0.0, i-0.25, j-0.5, 0.0, i+0.25,
									j+0.5, 0.0, i+0.25, j+0.5, 0.0, i-0.25, 
									j-0.5, 1.0, i-0.25, j-0.5, 1.0, i+0.25,
									j+0.5, 1.0, i+0.25, j+0.5, 1.0, i-0.25);
		addBBox(bbox_list, bbox_p1);
		if(h>0){
			bbox_t* bbox_p1 = createBBox(j-0.5, 0.0, i-0.25, j-0.5, 0.0, i+0.25,
									j+0.5, 0.0, i+0.25, j+0.5, 0.0, i-0.25, 
									j-0.5, h+1.0, i-0.25, j-0.5, h+1.0, i+0.25,
									j+0.5, h+1.0, i+0.25, j+0.5, h+1.0, i-0.25);
			addBBox(bbox_list, bbox_p1);
		}
	glPopMatrix();
}

void createBounded_longz(bbox_list_t* bbox_list, int i, int j, int h){
	glPushMatrix();
		bbox_t* bbox_p1 = createBBox(j-0.25, 0.0, i-0.5, j-0.25, 0.0, i+0.5,
									j+0.25, 0.0, i+0.5, j+0.25, 0.0, i-0.5, 
									j-0.25, 1.0, i-0.5, j-0.25, 1.0, i+0.5,
									j+0.25, 1.0, i+0.5, j+0.25, 1.0, i-0.5);
		addBBox(bbox_list, bbox_p1);
		if(h>0){
			bbox_t* bbox_p1 = createBBox(j-0.25, h+0.0, i-0.5, j-0.25, h+0.0, i+0.5,
									j+0.25, h+0.0, i+0.5, j+0.25, h+0.0, i-0.5, 
									j-0.25, h+1.0, i-0.5, j-0.25, h+1.0, i+0.5,
									j+0.25, h+1.0, i+0.5, j+0.25, h+1.0, i-0.5);
		addBBox(bbox_list, bbox_p1);
		}
	glPopMatrix();
}

void createBounded_all(bbox_list_t* bbox_list, int i, int j, int h){
	glPushMatrix();
		createBounded_longz(bbox_list, i, j, h);
		createBounded_longx(bbox_list, i, j, h);
	glPopMatrix();
}

bbox_list_t* createBBoxList(maze_t* maze){
	bbox_list_t* bbox_list = malloc(sizeof(bbox_list_t));
	bbox_list->size = 0;
	int i,j;
	GLint** lab = maze->maze;
	GLint rows = maze->rows;
	GLint cols = maze->cols;
	//~ printf("draw wall\n");
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			if(lab[i][j] && maze->type == STANDARD ){
				if( isInLab(i-1, j, rows, cols) && lab[i-1][j] &&
					isInLab(i+1, j, rows, cols) && lab[i+1][j] && 
					isInLab(i, j-1, rows, cols) && lab[i][j-1] &&
					isInLab(i, j+1, rows, cols) && lab[i][j+1])
					createBounded_all(bbox_list, i, j, maze->doubled);
				else if( isInLab(i+1, j, rows, cols) && lab[i+1][j] && 
						 isInLab(i, j-1, rows, cols) && lab[i][j-1] &&
						 isInLab(i, j+1, rows, cols) && lab[i][j+1])
					createBounded_tpz(bbox_list, i, j, maze->doubled);// -.-
				else if( isInLab(i-1, j, rows, cols) && lab[i-1][j] &&
						 isInLab(i+1, j, rows, cols) && lab[i+1][j] && 
						 isInLab(i, j+1, rows, cols) && lab[i][j+1])
					createBounded_tpx(bbox_list, i, j, maze->doubled);// |-
				else if( isInLab(i-1, j, rows, cols) && lab[i-1][j] &&
						 isInLab(i, j-1, rows, cols) && lab[i][j-1] &&
						 isInLab(i, j+1, rows, cols) && lab[i][j+1])
					createBounded_tnz(bbox_list, i, j, maze->doubled);// -'-
				else if( isInLab(i-1, j, rows, cols) && lab[i-1][j] &&
						 isInLab(i+1, j, rows, cols) && lab[i+1][j] && 
						 isInLab(i, j-1, rows, cols) && lab[i][j-1] )
					createBounded_tnx(bbox_list, i, j, maze->doubled);// -|
				else if( isInLab(i+1, j, rows, cols) && lab[i+1][j] &&
						 isInLab(i, j+1, rows, cols) && lab[i][j+1])
					createBounded_turn_pxpz(bbox_list, i, j, maze->doubled);// i-
				else if( isInLab(i-1, j, rows, cols) && lab[i-1][j] &&
						 isInLab(i, j+1, rows, cols) && lab[i][j+1])
					createBounded_turn_pxnz(bbox_list, i, j, maze->doubled);// L
				else if( isInLab(i-1, j, rows, cols) && lab[i-1][j] &&
						 isInLab(i, j-1, rows, cols) && lab[i][j-1])
					createBounded_turn_nxnz(bbox_list, i, j, maze->doubled);// _I
				else if( isInLab(i+1, j, rows, cols) && lab[i+1][j] &&
						 isInLab(i, j-1, rows, cols) && lab[i][j-1])
					createBounded_turn_nxpz(bbox_list, i, j, maze->doubled);// -i
				else if( (isInLab(i-1, j, rows, cols) && lab[i-1][j]) &&
						 (isInLab(i+1, j, rows, cols) && lab[i+1][j]) )
					createBounded_longz(bbox_list, i, j, maze->doubled);// | 
				else if( (isInLab(i-1, j, rows, cols) && lab[i-1][j]) &&
						 !isInLab(i+1, j, rows, cols) )
					createBounded_longz(bbox_list, i, j, maze->doubled);// | empty near border pz
				else if( (isInLab(i+1, j, rows, cols) && lab[i+1][j]) &&
						 !isInLab(i-1, j, rows, cols) )
					createBounded_longz(bbox_list, i, j, maze->doubled);// | empty near border nz
				else if( (isInLab(i-1, j, rows, cols) && lab[i-1][j]) &&
						 (isInLab(i+1, j, rows, cols) && !lab[i+1][j]) )
					createBounded_longz(bbox_list, i, j, maze->doubled);// | to n z
				else if( (isInLab(i-1, j, rows, cols) && !lab[i-1][j]) &&
						 (isInLab(i+1, j, rows, cols) && lab[i+1][j]) )
					createBounded_longz(bbox_list, i, j, maze->doubled);// | to p z
				else if( (isInLab(i, j-1, rows, cols) && lab[i][j-1]) &&
						 (isInLab(i, j+1, rows, cols) && lab[i][j+1]) )
					createBounded_longx(bbox_list, i, j, maze->doubled);// --
				else if( (isInLab(i, j-1, rows, cols) && lab[i][j-1]) &&
						 (!isInLab(i, j+1, rows, cols)) )
					createBounded_longx(bbox_list, i, j, maze->doubled);// -- empty near border px
				else if( (isInLab(i, j+1, rows, cols) && lab[i][j+1]) &&
						 (!isInLab(i, j-1, rows, cols)) )
					createBounded_longx(bbox_list, i, j, maze->doubled);// -- empty near border nx
				else if( (isInLab(i, j-1, rows, cols) && lab[i][j-1]) &&
						 (isInLab(i, j+1, rows, cols) && !lab[i][j+1]) )
					createBounded_longx(bbox_list, i, j, maze->doubled);// -- to n x
				else if( (isInLab(i, j-1, rows, cols) && !lab[i][j-1]) &&
						 (isInLab(i, j+1, rows, cols) && lab[i][j+1]) )
					createBounded_longx(bbox_list, i, j, maze->doubled);// -- to p x
				//~ else
					//~ drawWallBlock();
			} else
			if(lab[i][j] && maze->type == CRAZY ){
				createBoundedWallBlock(bbox_list, i, j, maze->doubled );
			}
		}
	}
	return bbox_list;
}

int hasCollitionBoxList(bbox_t* box, bbox_list_t* bbox_list){
	int i, s = 0;
	for( i=0; i<bbox_list->size; i++ ){
		s+=hasCollition(box, getBBox(bbox_list, i));
		s+=hasCollition(getBBox(bbox_list, i), box);
	}
	//~ printf("Collision %d\n", s);
	return s;
}
