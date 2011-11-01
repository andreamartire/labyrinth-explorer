//      menu.c
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"

void removeEndLine (char* s) {
    int i;
    //~ printf("Before:(%s)\n", s);
    for(i=0; i<strlen(s); i++){
		//~ printf("%c", s[i]);
		if (s[i] == '\n'){
			s[i] = '\0';
			//~ printf("Removing...\n");
		}
	}
	//~ printf("After:(%s)\n", s);
}

char* getFileNameToMaze(){
	FILE *fp;
	char* path = malloc(sizeof(char)*200);
	//~ path = "labyrinth14x14\n";
	fp = popen("zenity --title \"MazeGame by Andrea Martire\" --text \"Please choose\" --list --radiolist --column=Options --column= True \"New Maze\" False \"Load Maze\"", "r");
	if (fgets(path, 35, fp) != NULL){
		if(fp != NULL)
			pclose(fp);
		//~ printf("Choice: %s", path);
		if( strcmp(path, "New Maze\n") == 0){
			//~ printf("Nuovo Labirinto\n");
			//~ path = "labyrinth14x14\n";
			return "?GENERATE?";
		}
		else
		if( strcmp(path, "Load Maze\n") == 0 ){
			//~ printf("Carica Labirinto\n");
			FILE *fp2;
			fp2 = popen("zenity --file-selection", "r");
			if (fgets(path, 200, fp2) != NULL){
				if(fp2 != NULL)
					pclose(fp2);
				//~ printf("Carico labirinto nel file: %s.", path);
				removeEndLine(path);
				return path;
			}
			else {
				system(" zenity --error --text \"Error: Impossible load the file\"");
				return NULL;
			}
		}
	}
	
	//~ system(" zenity --error --text \"Error: Impossible to manage your choise\"");
	return NULL;
}

maze_t* getMaze(){
	char* fileName = getFileNameToMaze();
	maze_t* maze;
	
	if( strcmp("?GENERATE?", fileName) == 0 ){
		//~ printf("GENERATE...\n");
		FILE *pcols;
		char* data = malloc(sizeof(char)*5);
		pcols = popen("zenity --scale --text \"Choose columns's number:\"", "r");
		if (fgets(data, 5, pcols) != NULL){
			if(pcols != NULL)
				pclose(pcols);
			//~ printf("Letto numero di colonne: %s.", data);
		}
		int rows = atoi(data);
		pcols = popen("zenity --scale --text \"Choose rows's number:\"", "r");
		if (fgets(data, 5, pcols) != NULL){
			if(pcols != NULL)
				pclose(pcols);
			//~ printf("Letto numero di righe: %s.", data);
		}
		int cols = atoi(data);
		maze = (maze_t*) generate(rows,cols);
	}
	else 
		maze = loadMaze(fileName, STANDARD);
	return maze;
}
