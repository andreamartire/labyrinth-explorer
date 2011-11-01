#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "tga.h"
#include "utils.h"

#include "graphics.h"
#include "texture.h"
#include "player.h"
#include "maze.h"
#include "mouse.h"
#include "mytimer.h"
#include "bbox.h"
#include "bbox_list.h"
#include "hud.h"
#include "menu.h"

GLint textured = 1;
GLfloat alpha = 0;

texture_t *textureFloor;
texture_t *textureWall;
texture_t *textureOther;
texture_t *textureCompass;

player_t *player;
maze_t *maze;
hud_t* hud;

bbox_list_t *bbox_list;
mouse_t *mouse1;

char* fileName;
int** lab;
GLfloat fAspect;
GLint w = 1, h = 1;

// Light values and coordinates
GLfloat  ambientLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
GLfloat  noAmbientLight[] = { 0.1f, 0.1f, 0.1f, 1.0f };
GLfloat  diffuseLight[] = { 0.7f, 0.7f, 1.0f, 1.0f };
GLfloat	 lightPos[] = { 0.0f, 10.0f, 0.0f, 0.0f };
GLfloat playerPos[] = { 0.0, 0.5, 0.0, 1.0 };

// Change viewing volume and viewport.  Called when window is resized
void ChangeSize(int wl, int hl){
	//~ printf("Change size\n");
    w = wl;
    h = hl;
    // Prevent a divide by zero
    if(h == 0)
        h = 1;

    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);

    fAspect = (GLfloat)w/(GLfloat)h;
}

void SetupRC(){
    glEnable(GL_DEPTH_TEST);	// Hidden surface removal
    glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
    glEnable(GL_CULL_FACE);		// Do not calculate inside

    bbox_list = createBBoxList(maze);
    player = createPlayer(maze->ys, 0.3, maze->xs, 0, 0, 0.3f, bbox_list);
    mouse1 = createMouse(player);
    hud = createHud(maze, player, w, h);
    
    // Enable lighting
    glEnable(GL_LIGHTING);

    // Setup and enable light 0
    //~ glLightModelfv(GL_LIGHT_MODEL_AMBIENT,sourceLight);
    glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
    glEnable(GL_LIGHT0);
    
    glLightfv(GL_LIGHT1,GL_AMBIENT,noAmbientLight);
    glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuseLight);
    glLightfv(GL_LIGHT1,GL_POSITION,playerPos);
    //~ glEnable(GL_LIGHT1);

    // Enable color tracking
    glEnable(GL_COLOR_MATERIAL);
	
    // Set Material properties to follow glColor values
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    
	
    // Black blue background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    
    // Load texture
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
    textureFloor = createTexture("brick2.tga");
    textureWall = createTexture("brick2.tga");
    textureOther = createTexture("Stone.tga");
    textureCompass = createTexture("compass.tga");
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glEnable(GL_TEXTURE_2D);
}

void Keyboard(unsigned char key, int x, int y){
	
    switch (key)
	{
		case 'w':moveAhead(player, maze);break;
		case 's':moveBack(player, maze);break;
		case 'd':moveRight(player, maze);break;
		case 'a':moveLeft(player, maze);break;
		case 'z':player->y += 0.5;break;
		case 'x':player->y -= 0.5;break;
		case 'b':maze->bounded = !maze->bounded;break;
		case 'l':
				if( glIsEnabled(GL_LIGHT0) ){
					glEnable(GL_LIGHT1);
					glDisable(GL_LIGHT0);
				}
				else{
					glDisable(GL_LIGHT1);
					glEnable(GL_LIGHT0);
				}
				break;
		case 'k':if(maze->type == STANDARD)
					maze->type = CRAZY;
				else
					maze->type = STANDARD;
				double new_x = round(player->x);
				double new_z = round(player->z);
				//~ printf("X %f, Z %f -> X %f, Z %f\n", player->x, player->z,
						//~ player->x - player->x + new_x, player->z - player->z + new_z);
				moveBBox(player->box, new_x - player->x, 0, new_z - player->z);
				player->x = new_x;
				player->z = new_z;
				break;
		case 'c':
			player->firstPerson = !player->firstPerson;
			if(!player->firstPerson)
				player->radius = 1.0;
			break;
		case 27:
			deleteBBoxList(bbox_list);
			free(textureFloor);
			free(textureWall);
			free(textureOther);
			free(lab);
			
			FILE *stream;
			char buffer[10];
			char* msg = malloc(sizeof(char)*20);

			stream = popen("ps ax eopid,comm | grep -v grep | grep mplayer | cut -c 1-6", "r");
			while ( fgets(buffer, 10, stream) != NULL ){
				printf("%s", buffer);
				sprintf(msg, "kill -9 %s", buffer);
				system(msg);
				printf("Killed: %s", msg, buffer);
			}
			pclose(stream);
			
			exit(0);
			break;
	}
	
	glutPostRedisplay();
}

void mouse( int x, int y ){
	updateMouse(mouse1, x, y);
	glutPostRedisplay();
}

// Called to draw scene
void RenderScene(void){
	
	alpha = alpha + 0.25;
	
    GLTVector3 vNormal;
             
    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho (0, w, 0, h, 1, -1);
	
		updateHud(hud,w,h);
		glTexImage2D(GL_TEXTURE_2D, 0, textureCompass->components, textureCompass->width,
						textureCompass->height, 0, textureCompass->format, 
						GL_UNSIGNED_BYTE, textureCompass->texture);
		
		drawHud(hud, w, h);		
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0f, fAspect, 0.001, 1000.0);			
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glColor3f(1.0f, 0.0f, 0.0f);
	
	glEnable(GL_LIGHTING);
	
	glPushMatrix();
    	
		showPlayer(player);
			
		glLightfv(GL_LIGHT1, GL_POSITION, playerPos);
		
		glTranslatef(maze->cols/2, 0.0f, maze->rows/2);
		
		glTranslatef(-player->x, -player->y, -player->z);
        
		drawAxis();
        
        glColor3f(1.0f, 1.0f, 1.0f);
        
        glPushMatrix();
        
			glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
        
			drawCubeWorld(maze->rows, maze->cols, (int) alpha);
			
			glTranslatef((float) (-maze->cols/2), 0, (float) (-maze->rows/2));
			
			glTexImage2D(GL_TEXTURE_2D, 0, textureWall->components, textureWall->width,
					textureWall->height, 0, textureWall->format, 
					GL_UNSIGNED_BYTE, textureWall->texture);
			
			drawLabyrinth(maze, bbox_list);
			
			glEnable(GL_TEXTURE_2D);
			glTexImage2D(GL_TEXTURE_2D, 0, textureFloor->components, textureFloor->width,
					textureFloor->height, 0, textureFloor->format, 
					GL_UNSIGNED_BYTE, textureFloor->texture);
					
			drawLabyrinthFloor(maze);
			
			glTexImage2D(GL_TEXTURE_2D, 0, textureOther->components, textureOther->width,
					textureOther->height, 0, textureOther->format, 
					GL_UNSIGNED_BYTE, textureOther->texture);
					
			//~ drawLabyrinthOtherFloor(maze);
		glPopMatrix();
		
		
		
    // Restore the matrix state
    glPopMatrix();

    // Buffer swap
    glutSwapBuffers();
}

enum
{
	MOUSE_LEFT_BUTTON = 0,
	MOUSE_MIDDLE_BUTTON = 1,
	MOUSE_RIGHT_BUTTON = 2,
	MOUSE_SCROLL_UP = 3,
	MOUSE_SCROLL_DOWN = 4
};

void mouseWheel(int button, int state, int x, int y)
{
	switch (button)
	{
		// zoom in
		case MOUSE_SCROLL_UP:
			player->radius += 0.5;
			break;

		// zoom out
		case MOUSE_SCROLL_DOWN:
			player->radius -= 0.5;
			break;
	}
	
	GLint maxsize;
	
	if( maze->rows > maze->cols )
		maxsize = maze->rows;
	else
		maxsize = maze->cols;
	
	if( player->radius < 1.0 )
		player->radius = 1.0;
	else if( player->radius > maxsize )
		player->radius = maxsize;
	
	glutPostRedisplay();
	
	return;
}

void update(){
	RenderScene();
	return;
}

int main(int argc, char *argv[])
{
	maze = getMaze();
	//~ maze = (maze_t*) generate(16,15);
	
	if( maze != NULL ){
		printf("Ok loaded\n");
		initPositionMaze(maze);
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		//~ /*glutInitWindowSize(800, 600);
		//~ glutCreateWindow("Textured Maze");*/
		glutGameModeString( "1280x800:24" ); //the settings for fullscreen mode
		glutEnterGameMode(); //set glut to fullscreen
		glutReshapeFunc(ChangeSize);
		glutDisplayFunc(RenderScene);
		SetupRC();
		
		glutIdleFunc(update);
		
		glutKeyboardFunc(Keyboard);
		glutPassiveMotionFunc(mouse);
		glutMouseFunc(mouseWheel);
		glutSetCursor(GLUT_CURSOR_NONE);
		system("mplayer -loop 99999 intro.ogg &");
		glutMainLoop();
		glutLeaveGameMode();
	}
	else
		printf("Doesn't loaded\n");
	
    return 0;
}
