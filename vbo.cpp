/*
 *  vbo.cpp
 *  lorenz
 *
 *  Created by McK on 10/02/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <stdio.h>
#include <OpenGL/gl.h>
#include "vbo.h"

GLuint vboId=0;
	
void createVBO(int size) {
	destroyVBO();
	glGenBuffersARB(1, &vboId);
	printf("BUFFERID: %d\n", vboId);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, size*sizeof(Vertex), 0, GL_STATIC_COPY_ARB);
	
}

void updateVBO(Vertex *verts, int numvers) {
	createVBO(numvers);
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, numvers*sizeof(Vertex), 0, GL_STATIC_COPY_ARB);
	glBufferSubDataARB(GL_ARRAY_BUFFER_ARB, 0, numvers*sizeof(Vertex), verts);
}


void destroyVBO() {
	if(vboId) {
		glDeleteBuffersARB(1, &vboId);
	}
	vboId = 0;
}


void drawVBO(int start, int numvers, int line) {
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	
	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), 0);
	glColorPointer(4, GL_FLOAT, sizeof(Vertex), (char *)(3*sizeof(float)));
		
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable (GL_BLEND);
    glEnable(GL_ALPHA);
    
	if(line==0) {
		glDrawArrays(GL_TRIANGLE_STRIP, start, numvers);
	} else if(line==1){
		glEnable(GL_POINT_SMOOTH);
		glPointSize(5.0);
		
		glDrawArrays(GL_POINTS, start, numvers);
        glPointSize(1.0);
    } else if(line==2){
        glLineWidth(3.0f);
        
        glDrawArrays(GL_LINE_STRIP, start, numvers);
        glLineWidth(1.0f);
    }else{
        glDrawArrays(GL_LINES, start, numvers);
    }

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
					
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}


void drawVBOComponents(int start, int numvers) {
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, vboId);
	
	Vertex *verts = (Vertex *)glMapBufferARB(GL_ARRAY_BUFFER_ARB, GL_READ_ONLY_ARB);
	
	double t = 0;
	double step = 60.0/(numvers);

	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(60.0, 0.0, 0.0);
	glEnd();
	
	glBegin(GL_LINE_STRIP);
	for(int i=start; i<(numvers+start);i++) {
	    //glColor4f(verts[i].colour[0], verts[i].colour[1], verts[i].colour[2], 1.0);
		glColor4f(1.0, 0.0, 0.0, 1.0);
		glVertex3d(t, verts[i].position[0]/4, 0);			
		t+=step;
	}
	glEnd();
	
	glTranslated(0, -10, 0);
	
	t=0;
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(60.0, 0.0, 0.0);
	glEnd();
	
	glBegin(GL_LINE_STRIP);
	for(int i=start; i<(numvers+start);i++) {
	    //glColor4f(verts[i].colour[0], verts[i].colour[1], verts[i].colour[2], 1.0);
		glColor4f(0.0, 1.0, 0.0, 1.0);
		glVertex3d(t, verts[i].position[1]/4, 0);			
		t+=step;
	}
	glEnd();
	
	glTranslated(0, -20, 0);
	
	t=0;
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glBegin(GL_LINE_STRIP);
	glVertex3d(0.0, 0.0, 0.0);
	glVertex3d(60.0, 0.0, 0.0);
	glEnd();
	
	glBegin(GL_LINE_STRIP);
	for(int i=start; i<(numvers+start);i++) {
	    //glColor4f(verts[i].colour[0], verts[i].colour[1], verts[i].colour[2], 1.0);
		glColor4f(0.0, 0.0, 1.0, 1.0);
		glVertex3d(t, verts[i].position[2]/4, 0);			
		t+=step;
	}
	glEnd();
	
	glUnmapBuffer(GL_ARRAY_BUFFER_ARB);
	
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
}

