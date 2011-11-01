#include "maze.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

maze_t* loadMaze( char *str, MAZE_TYPE type){
	if ( str == NULL )
		return NULL;
	maze_t* lab = malloc(sizeof(maze_t));
	FILE *file; 
	char* buffer;
	int fileDim;
	int ROWS, COLS;
	if(( file = fopen( str, "rb")) != NULL)
	{
		// Si posiziona alla fine del file
		fseek(file, 0, SEEK_END);
		// Legge la posizione attuale
		fileDim = ftell(file);
		// Alloca la dimensione del buffer
		buffer = (char*) malloc(sizeof(char) * (fileDim+1));
		//~ printf("Dimensione del file = %d\n", fileDim);
		// Mi riporto all'inizio del file
		fseek(file, 0, SEEK_SET);
		// Copio tutto il contenuto del file nel buffer
		fread(buffer, fileDim, 1, file);
		// Chiudo il file
		if( file != NULL)
			fclose(file);
	}
	
	int i = 0, j = 0;
	char* pch = (char*) strtok (buffer,"\t\n\r ");
	if( pch != NULL )
		ROWS = atoi(pch);
	pch = (char*) strtok (NULL,"\t\n\r ");
	if( pch != NULL )
		COLS = atoi(pch);
	
	//~ printf("ROWS: %d, COLS: %d\n", ROWS, COLS);
	
	if(ROWS <= 0 || COLS <= 0){
		system("zenity --error --text \"Error: Malformed file\"");
		return NULL;
	}
	
	GLint** m;
	m = malloc(sizeof(float*)*ROWS);
	m[i] = malloc(sizeof(float)*COLS);
	
	pch = (char*) strtok (NULL,"\t\n\r ");
	int complete = 0;
	
	while (pch != NULL && !complete )
	{
		//~ printf ("VALORE %s\n",pch);
		if(pch != NULL){			
			m[i][j] = atoi(pch);
			//~ printf("SALVO %d in %d,%d\n", m[i][j], i, j);
			if( j+1 == COLS ){
				j = 0;
				if( i+1 != ROWS ){
					//~ printf("\n");
					i++;
					m[i] = malloc(sizeof(float)*COLS);
				}
				else
					complete = 1;
			}
			else
				j++;
		}
		pch = (char*) strtok (NULL, "\t\n\r ");
	}
	
	lab->maze = m;
	lab->rows = ROWS;
	lab->cols = COLS;
	lab->mytimer = malloc(sizeof(mytimer_t));
	lab->mytimer = createTimer(ROWS*COLS/4);
	lab->viewGraphic = 0;
	lab->doubled = 1;
	lab->type = type;
	lab->oldtype = type;
	lab->bounded = 0;
	
	if(!complete){
		system("zenity --error --text \"Error: Incomplete file\"");
		return NULL;
	}
	return lab;
}

void showMaze(maze_t* maze){
	int i,j;
	printf("Show Maze: ");
	if( maze->type == STANDARD )
		printf("STANDARD\n");
	else
		printf("CRAZY\n");
	for( i=0; i<maze->rows; i++){
		for( j=0; j<maze->cols; j++){
			printf("%d ", (int) maze->maze[i][j]);
		}
		printf("\n");
	}
}

void show(int** maze, int rows, int cols){
	int i,j;
	printf("Show Matrix:\n");
	for( i=0; i<rows; i++){
		for( j=0; j<cols; j++){
			printf("%d ", maze[i][j]);
		}
		printf("\n");
	}
}

void initPositionMaze(maze_t* maze){
	int i, j, counter = 0;
	for( i=0; i<maze->rows; i++){
		for( j=0; j<maze->cols; j++){
			if(i == 0 || i == maze->rows-1 || j == 0 || j == maze->cols-1 ){
				if(!maze->maze[i][j]){
					if(counter == 0){
						maze->xs = i;
						maze->ys = j;
					}
					if(counter == 1){
						maze->xf = i;
						maze->yf = j;
					}
					counter++;
				}
			}
		}
	}
	printf("Founded %d holes\n", counter);
	printf("Start %d,%d\n", maze->xs, maze->ys);
	printf("Finish %d,%d\n", maze->xf, maze->yf);
}

int isOdd(int n){
	if(n % 2 == 1 )
		return 1;
	return 0;
}

int isSafeRange(int i, int j, int rows, int cols){
	if( i>=0 && i<rows && j>=0 && j<cols)
		return 1;
	return 0;
}

int** generate_lab(int rows, int cols){
	int i, j;
	int** m;
	m = malloc(sizeof(float*)*rows);
	for(i = 0; i < rows; i++){
		m[i] = malloc(sizeof(float)*cols);
		for( j = 0; j < cols; j++ ){
			m[i][j] = 0;
		}
	}
	
	// algoritmo di generazione
	
	// Genero punto casuale che è un muro rimovibile
	srand(time(0));
	int ir, jr;
	int isComplete = 1;
	
	do{	
		// inizializzo a valori dispari
		ir = 0, jr = 0;
		
		// genero celle con coordinate dispari
		do 
			ir = (rand() % (rows - 2) ) + 1;
		while( !isOdd(ir) );
		do
			jr = (rand() % (cols - 2) ) + 1;
		while( !isOdd(jr) );
		//~ printf("Generated %d,%d\n", ir, jr);
		
		int canChooseDirection;
		
		do{
			canChooseDirection = 0;
			int i_off[4], j_off[4], count = 0;
			if( isSafeRange(ir + 2, jr, rows-1, cols-1) && m[ir + 2][jr] == 0 ){
				canChooseDirection++;
				i_off[count] = 2;
				j_off[count++] = 0;
				//~ m[ir + 2][jr] = 8;
			}
			if( isSafeRange(ir - 2, jr, rows-1, cols-1) && m[ir - 2][jr] == 0 ){
				canChooseDirection++;
				i_off[count] = -2;
				j_off[count++] = 0;
				//~ m[ir - 2][jr] = 8;
			}
			if( isSafeRange(ir, jr + 2, rows-1, cols-1) && m[ir][jr + 2] == 0 ){
				canChooseDirection++;
				i_off[count] = 0;
				j_off[count++] = 2;
				//~ m[ir][jr + 2] = 8;
			}
			if( isSafeRange(ir, jr - 2, rows-1, cols-1) && m[ir][jr - 2] == 0 ){
				canChooseDirection++;
				i_off[count] = 0;
				j_off[count++] = -2;
				//~ m[ir][jr - 2] = 8;
			}
			
			//~ printf("%d,%d Available direction: %d\n", ir, jr, canChooseDirection );
			if( count > 0 )
				//~ for( i = 0; i < count; i++ )
					//~ printf("Direction %d,%d\n", i_off[i], j_off[i] );
			
			if( canChooseDirection ){
				int r = rand() % canChooseDirection;
				//~ printf("Choosed Direction: %d,%d\n", i_off[r], j_off[r] );
				// apply direction on target cell
				int i_old_off = i_off[r], j_old_off = j_off[r];
				m[ir + i_off[r]][jr + j_off[r]] = 1;
				// apply direction on intermediate cell
				if( i_off[r] == 2 )
					i_off[r]--;
				else if( i_off[r] == -2 )
					i_off[r]++;
				else if( j_off[r] == 2 )
					j_off[r]--;
				else if( j_off[r] == -2 )
					j_off[r]++;
				m[ir + i_off[r]][jr + j_off[r]] = 1;
				// set new current cell
				ir += i_old_off;
				jr += j_old_off;
			}
		}
		while(canChooseDirection);
		
		isComplete = 1;
		for( i = 0; i < rows; i++ ){
			for( j = 0; j < rows; j++ ){
				if(!m[i][j]){ // se non e' un muro
					// verifico che un suo vicino sia un muro
					int nWall = 0;
					if( isSafeRange(i, j - 1, rows-1, cols-1) && m[i][j - 1] == 1 )
						nWall++;
					if( isSafeRange(i, j + 1, rows-1, cols-1) && m[i][j + 1] == 1 )
						nWall++;
					if( isSafeRange(i + 1, j, rows-1, cols-1) && m[i + 1][j] == 1 )
						nWall++;
					if( isSafeRange(i - 1, j, rows-1, cols-1) && m[i - 1][j] == 1 )
						nWall++;
					if( isSafeRange(i + 1, j - 1, rows-1, cols-1) && m[i + 1][j - 1] == 1 )
						nWall++;
					if( isSafeRange(i + 1, j + 1, rows-1, cols-1) && m[i + 1][j + 1] == 1 )
						nWall++;
					if( isSafeRange(i - 1, j + 1, rows-1, cols-1) && m[i - 1][j + 1] == 1 )
						nWall++;
					if( isSafeRange(i - 1, j - 1, rows-1, cols-1) && m[i - 1][j - 1] == 1 )
						nWall++;
					if(nWall == 0)
						isComplete = 0;
				}
			}
		}
	}
	while(!isComplete);
	
	return m;
} 

int** generateNewLab(int rows, int cols){
	int i, j;
	int** m;
	m = malloc(sizeof(int*)*rows);
	for(i = 0; i < rows; i++){
		m[i] = malloc(sizeof(int)*cols);
		for( j = 0; j < cols; j++ ){
			m[i][j] = 0;
		}
	}
	
	int* wx, *wy;
	wx = malloc(sizeof(int)*rows*cols);
	wy = malloc(sizeof(int)*rows*cols);
	for(i = 0; i < rows*cols; i++){
		wx[i] = -1;
		wy[i] = -1;
	}
	int sw = 0;
	
	// algoritmo di generazione
	
	// Creo muri esterni - Aggiungo in lista solo muri 'dispari'
	// per ottenere labirinti dove non siano presenti muri paralleli attaccati
	for( j = 0; j < cols; j++ ){
		m[0][j] = 1;
		if( !isOdd(j) ){
			wx[sw] = 0;
			wy[sw] = j;
			sw++;
		}
		
		m[rows-1][j] = 1;
		if( !isOdd(rows-1) && !isOdd(j) ){
			wx[sw] = rows-1;
			wy[sw] = j;
			sw++;
		}
	}
	for( i = 2; i < rows-1; i++ ){
		m[i][0] = 1;
		if( !isOdd(i) ){
			wx[sw] = i;
			wy[sw] = 0;
			sw++;
		}
	}
	for( i = 1; i < rows-2; i++ ){
		m[i][cols-1] = 1;
		if( !isOdd(i) && !isOdd(cols-1) ){
			wx[sw] = i;
			wy[sw] = cols-1;
			sw++;
		}
	}
	
	for( i = 0; i < sw; i++ ){
		//~ printf("(%d,%d)\n", wx[i], wy[i]);
	}
	
	srand(time(0));
	
	int index, steps, max_step = (int)((rows+cols)/4);
	if(max_step == 0)
		max_step++;
	int counter = 0, isComplete = 0;
	
	// Genero un muro casuale fra quelli esistenti
	
	do{
		index = rand() % sw;
		steps = (rand() % max_step) + 1;
		
		//~ printf("Generate: %d (%d,%d)\n", index, wx[index], wy[index]);
		int ir = wx[index];
		int jr = wy[index];	
	
		do {
			//~ m[ir][jr] = 2;
			//~ printf("Calcolo direzioni disponibili: \n");
			int canChooseDirection = 0;
			int i_off[4], j_off[4], count = 0;
			if( isSafeRange(ir + 2, jr, rows-1, cols-1) && m[ir + 2][jr] == 0 ){
				canChooseDirection++;
				i_off[count] = 2;
				j_off[count++] = 0;
				//~ m[ir + 2][jr] = 8;
			}
			if( isSafeRange(ir - 2, jr, rows-1, cols-1) && m[ir - 2][jr] == 0 ){
				canChooseDirection++;
				i_off[count] = -2;
				j_off[count++] = 0;
				//~ m[ir - 2][jr] = 8;
			}
			if( isSafeRange(ir, jr + 2, rows-1, cols-1) && m[ir][jr + 2] == 0 ){
				canChooseDirection++;
				i_off[count] = 0;
				j_off[count++] = 2;
				//~ m[ir][jr + 2] = 8;
			}
			if( isSafeRange(ir, jr - 2, rows-1, cols-1) && m[ir][jr - 2] == 0 ){
				canChooseDirection++;
				i_off[count] = 0;
				j_off[count++] = -2;
				//~ m[ir][jr - 2] = 8;
			}
			
			if( canChooseDirection ){
				int r = rand() % canChooseDirection;
				//~ printf("Choosed Direction: %d,%d\n", i_off[r], j_off[r] );
				// apply direction on target cell
				int i_old_off = i_off[r], j_old_off = j_off[r];
				m[ir + i_off[r]][jr + j_off[r]] = 1;
				// apply direction on intermediate cell
				if( i_off[r] == 2 )
					i_off[r]--;
				else if( i_off[r] == -2 )
					i_off[r]++;
				else if( j_off[r] == 2 )
					j_off[r]--;
				else if( j_off[r] == -2 )
					j_off[r]++;
				m[ir + i_off[r]][jr + j_off[r]] = 1;
				// set new current cell
				ir += i_old_off;
				jr += j_old_off;
					if( !isOdd(ir) && !isOdd(jr) ){
					wx[sw] = ir;
					wy[sw] = jr;
					sw++;
					//~ m[ir][jr] = 7;
				}
			}
			else{
				//~ printf("Nessuna direzione disponibile. Muri: %d\n", sw);
				//index da eliminare
				//sw da shiftare
				for(i=index; i<sw-1; i++){
					wx[i] = wx[i+1];
					wy[i] = wy[i+1];
				}
				sw--;
			}
					
			isComplete = 1;
			for( i = 0; i < rows; i++ ){
				for( j = 0; j < cols; j++ ){
					if(!m[i][j]){ // se non e' un muro
						// verifico che un suo vicino sia un muro
						int nWall = 0;
						if( isSafeRange(i, j - 1, rows, cols) && m[i][j - 1] == 1 )
							nWall++;
						if( isSafeRange(i, j + 1, rows, cols) && m[i][j + 1] == 1 )
							nWall++;
						if( isSafeRange(i + 1, j, rows, cols) && m[i + 1][j] == 1 )
							nWall++;
						if( isSafeRange(i - 1, j, rows, cols) && m[i - 1][j] == 1 )
							nWall++;
						if( isSafeRange(i + 1, j - 1, rows, cols) && m[i + 1][j - 1] == 1 )
							nWall++;
						if( isSafeRange(i + 1, j + 1, rows, cols) && m[i + 1][j + 1] == 1 )
							nWall++;
						if( isSafeRange(i - 1, j + 1, rows, cols) && m[i - 1][j + 1] == 1 )
							nWall++;
						if( isSafeRange(i - 1, j - 1, rows, cols) && m[i - 1][j - 1] == 1 )
							nWall++;
						if(nWall == 0)
							isComplete = 0;
					}
				}
			}
			
			counter++;
		}
		while(counter < max_step);
	}
	while(!isComplete);
	
	show(m,rows, cols);
	
	return m;
}

maze_t* generate( int r, int c ){
	if ( r <= 1 )
		r = 2;
	if ( c <= 1 )
		c = 2;
	if ( r % 2 == 0 )
		r++;
	if ( c % 2 == 0 )
		c++;
	maze_t* lab = malloc(sizeof(maze_t));
	lab->rows = r;
	lab->cols = c;
	lab->mytimer = malloc(sizeof(mytimer_t));
	lab->mytimer = createTimer(r*c/4);
	lab->viewGraphic = 0;
	lab->doubled = 1;
	lab->type = 0;
	lab->oldtype = 0;
	lab->bounded = 0;
	lab->maze = generateNewLab(r,c);
	initPositionMaze(lab);
	return lab;
}
