/*
 *  drawing.cpp
 *  lorenz
 *
 *  Created by McK on 17/02/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/CGLDevice.h>
#include <GLUT/glut.h>
#include "drawing.h"


void GetColour(double v,double vmin,double vmax, COLOUR *c)
{
	c->r = 1.0;
	c->g = 1.0;
	c->b = 1.0;
	
	if (v < vmin)
		v = vmin;
	if (v > vmax)
		v = vmax;
	
	double dv = vmax - vmin;
	
	if (v < (vmin + 0.25 * dv)) {
		c->r = 0;
		c->g = 4 * (v - vmin) / dv;
	} else if (v < (vmin + 0.5 * dv)) {
		c->r = 0;
		c->b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
	} else if (v < (vmin + 0.75 * dv)) {
		c->r = 4 * (v - vmin - 0.5 * dv) / dv;
		c->b = 0;
	} else {
		c->g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
		c->b = 0;
	}
}



void drawCoords() {
	glColor4f(1, 0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glEnd();
	
	glColor4f(0, 1, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glEnd();
	
	glColor4f(0, 0, 1, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();
	
	glColor4f(1,0,0,1);
	
	//glutSolidSphere(1, 20, 20);
}


void drawText(char *cadena, float x, float y, float z)
{
	char *c;
	
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glColor4f(1,1,1,1);
	glRasterPos3f(x, y, z);
	for (c = cadena; *c; c++)
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
	}
	//glEnable(GL_LIGHTING);
	//glEnable(GL_DEPTH_TEST);
}
