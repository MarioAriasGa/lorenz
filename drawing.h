/*
 *  drawing.h
 *  lorenz
 *
 *  Created by McK on 17/02/10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */


typedef struct {
	double r,g,b,a;
} COLOUR;

void GetColour(double v,double vmin,double vmax, COLOUR *c);
void drawCoords();

void drawText( char *cadena, float x, float y, float z);
