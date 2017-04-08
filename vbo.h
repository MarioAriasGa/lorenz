/*
 *  vbo.h
 *  lorenz
 *
 *  Created by McK on 10/02/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

// Location/Normals
#define X_POS 0
#define Y_POS 1
#define Z_POS 2

// Texture Coordinates
#define U_POS 0
#define V_POS 1

// Colours
#define R_POS 0
#define G_POS 1
#define B_POS 2
#define A_POS 3

// A helper macro to get a position
#define BUFFER_OFFSET(i) ((char *)(i))

// Vertex structure
typedef struct
{
	GLfloat position[3];
	//GLfloat tex[2];
	//GLfloat normal[3];
	GLfloat colour[4];
//	GLubyte padding[1]; // Total: 28 ?
} Vertex;




void createVBO(int size);

void updateVBO(Vertex *verts, int numvers);
void destroyVBO();

void drawVBO(int start, int numvers, int line);
void drawVBOComponents(int start, int numvers);