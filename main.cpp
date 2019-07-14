#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#include <OpenGL/CGLDevice.h>
#include <GLUT/glut.h>
#elif __linux__
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include "vector.hpp"

#include "drawing.h"
#include "solver.h"
#include "vbo.h"

#define TIMER_DELAY 25
#define WIDTH 1280
#define HEIGHT 800
#define POSX 10
#define POSY 10

#define MAX_POINTS 1000000
#define MAX_SKIP 10

#define NUM_SHOW 1000

// Camera
double rotx = 0;
double roty = 0;
double zoom = 0.8;
double autorotate = 0.2;
int screenWidth = WIDTH;
int screenHeight = HEIGHT;

int fps = 0;
char fps_str[10]="";
char param_str[500]="";

// Status
bool running = 1;
int showAll = 1;
int showComponents = 1;
int lineMode = 1;
int moveIncrement = 100;

int counter = 0;

int usingVBO=1;

double minx, maxx, miny, maxy, minz, maxz;
double alpha=0.8;

// Runge Kutta
extern int solver;
extern char *solverNames[];
extern double error;
extern double minh;
extern double maxh;
extern double dt;
extern double t;
extern int skip;
extern int points;
extern Vector currentPoint;
extern Vector initial;
extern double sigma;
extern double b;
extern double r;


void fillVBO() {

	printf("Filling VBO\n");

	Vertex *vertices = (Vertex *) malloc(points*sizeof(Vertex));
	COLOUR c;
	
	currentPoint = initial;	
	t=0.0;
	minx = miny = minz = 10000000000;
	maxx = maxy = maxz = -10000000000;
	
	for (int i = 0; i < points; i++) {
		step(t, currentPoint, dt);
		
		vertices[i].position[0] = currentPoint[1];
		vertices[i].position[1] = currentPoint[2];
		vertices[i].position[2] = currentPoint[3];
//		vertices[i].position[3] = 1;//currentPoint[4];
		
		minx = min(vertices[i].position[0], minx);
		maxx = max(vertices[i].position[0], maxx);

		miny = min(vertices[i].position[1], miny);
		maxy = max(vertices[i].position[1], maxy);
		
		minz = min(vertices[i].position[2], minz);
		maxz = max(vertices[i].position[2], maxz);
		
		GetColour(i, 0, points, &c);
		vertices[i].colour[0] = c.r;
		vertices[i].colour[1] = c.g;
		vertices[i].colour[2] = c.b;
		vertices[i].colour[3] = alpha;
		
		for (int j=0; j<skip; j++) {
			step(t, currentPoint, dt);
		}
	}
	
	updateVBO(vertices, points);
	
	free(vertices);
	
	printf("VBO Filled OK\n");
}

void drawDirect() {
	/* render the scene here */
	currentPoint = initial;
	
	COLOUR c;
	
    glPushMatrix();
    
    glLoadIdentity();
    
    // Rotar la camara
    glRotatef(-rotx,1,0,0);
    glRotatef(roty,0,1,0);
    
	glScaled(0.7, 0.7, 0.7);
	glTranslated(0, 0, -20);
	
	
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);
	glEnable (GL_BLEND);
	glEnable(GL_ALPHA);
    glLineWidth(2.0);
	
    skip=0;
    dt=0.001;
    
    glBegin(GL_LINE_STRIP);
	t=0.0;
    
	for (int i = 0; i <= points; i++) {
		for (int j=0; j<skip; j++) {
			step(t, currentPoint, dt);
		}
		step(t, currentPoint, dt);
		
		GetColour(i, 0, points, &c);
		glColor4f(c.r, c.g, c.b, alpha);
		glVertex3d(currentPoint[1], currentPoint[2], currentPoint[3]);
	}
    glEnd();
	
	glDisable(GL_LINE_SMOOTH);
    
    glPopMatrix();
}


void drawFigure() {
	glPushMatrix();
	
	glLoadIdentity();
	
	// Rotar la camara 
	glRotatef(-rotx,1,0,0);
	glRotatef(roty,0,1,0);
	
	
	//printf("(%f,%f) (%f,%f) (%f,%f)\n", minx, maxx, miny, maxy, minz, maxz);
	
	drawCoords();
	
	glScaled(zoom, zoom, zoom);
	
	//glTranslated(-(maxx-minx)/2, 0, -(maxz-minz)/2);
	glTranslated(0, 0, -(maxz-minz)/2);
	//glTranslated(0, 0, -20);
	
	if(showAll==0) {
		drawVBO(0, points, lineMode);
	} else if(showAll==1){
		drawVBO(0, counter, lineMode);
	} else {
		drawVBO(counter, NUM_SHOW, lineMode);
	}
	
	glPopMatrix();
}

void drawComponents() {
	glPushMatrix();
	glLoadIdentity();
	glTranslated(-30, 10, 0);
	if(showAll==0) {
		drawVBOComponents(0, points);
	} else if(showAll==1) {
		drawVBOComponents(0, counter);
	} else {
		drawVBOComponents(counter, NUM_SHOW);
	}
	glPopMatrix();
}

/* render the scene */
void draw() {
	static int lastime=0;
	fps++;
	if(running) {
		counter+=moveIncrement;
		counter = counter % (points-NUM_SHOW);
        roty += autorotate;
	}
	
	if(time(NULL)-lastime>2) {
		sprintf(fps_str,"FPS: %d",fps/2);
		lastime = time(NULL);
		fps=0;
	}
	
	glClear(GL_COLOR_BUFFER_BIT);

	if(!(showComponents & 1)) {
		drawComponents();
	}	
	
	if(!(showComponents & 2)) {
        if(usingVBO) {
            drawFigure();
        } else {
            drawDirect();
        }
	}
	
	drawText(fps_str, -29.5, -19.5, 0);
	
	sprintf(param_str,"solver=%d points=%d  skip=%d  dt=%.3f  T=%.2f  sigma=%.2f  b=%.2f  r=%.2f  (%.2f,%.2f,%.2f), running=%s",solver, points, skip, dt, t, sigma, b,r,initial[1], initial[2], initial[3], running ? "Y" : "n");
	drawText(param_str, -15, -19.5, 0);
	
	glutSwapBuffers();
}


void timer(int valor) {
    if(running) {
        glutPostRedisplay();
        glutTimerFunc( TIMER_DELAY, timer, 1);
    }
}

/* executed when a regular key is pressed */
void keyboardDown(unsigned char key, int x, int y) {
    printf("Key: %d\n",key);
	switch(key) {
		case '1':
			dt = 0.001;                // time step for integration
			skip = 1;                     // steps to skip
			points = 50000;
			sigma = 10;
			b = 8.0 / 3.0;
			r = 28;
			
			initial[1]= 1.0;
			initial[2] = -2.0;
			initial[3] = 1.0;
            
			lineMode=1;
			counter=0;
			alpha=1.0;
			moveIncrement=100;
			showAll=0;
			
			fillVBO();
			break;
			
		case '2':
			dt = 0.001;
			points = 50000;
			skip = 2;
			initial[1] = 5;
			initial[2] = -30;
			initial[3] = 5;
			
			// Lorenz
			sigma = 10;
			b = 8.0 / 3.0;
			r = 28;

			lineMode=1;
			counter=0;
			alpha=0.2;
			moveIncrement=100;
			showAll=0;
			
			fillVBO();
			break;
		case '3':
			dt = 0.005;                // time step for integration
			skip = 4;                     // steps to skip
			points = 100000;
			sigma = 10;
			b = 8.0 / 3.0;
			r = 28;
			
			initial[1]= 0.0;
			initial[2] = 10.0;
			initial[3] = 0.0;

			lineMode=0;
			counter=0;
			alpha=0.08;
			moveIncrement=100;
			showAll=1;
			
			fillVBO();
			break;
		case '4':
			dt = 0.001;
			points = 100000;
			skip = 1;
			initial[1] = 5;
			initial[2] = -30;
			initial[3] = 5;
			
			// Lorenz
			sigma = 10;
			b = 8.0 / 3.0;
			r = 28;

			lineMode=2;
			counter=0;
			alpha=1;
			moveIncrement=50;
			showAll=1;
			
			fillVBO();
			break;
		case '5':
			dt = 0.001;
			points = 100000;
			skip = 1;
			initial[1] = 5.1;
			initial[2] = -29.9;
			initial[3] = 5.1;
			
			// Lorenz
			sigma = 10;
			b = 8.0 / 3.0;
			r = 28;
			
			lineMode=2;
			counter=0;
			running=1;
			alpha=0.8;
			moveIncrement=50;
			showAll=2;

			fillVBO();
			break;			
		case 'a':
			sigma*=1.2;
			fillVBO();
			break;
			
		case 'z':
			sigma/=1.2;
			fillVBO();
			break;
			
		case 's':
			b*=1.2;
			fillVBO();			
			break;
			
		case 'x':
			b/=1.2;
			fillVBO();			
			break;
			
		case 'd':
			r*=1.2;
			fillVBO();			
			break;
			
		case 'c':
			r/=1.2;
			fillVBO();			
			break;
			
		case 'f':
			dt*=2;
			fillVBO();			
			break;
			
		case 'v':
			dt/=2;
			fillVBO();			
			break;
		case 'g':
			points *=2;
			points = points>MAX_POINTS ? MAX_POINTS : points; 
			fillVBO();
			break;
		case 'b':
			points /=2;
			points = points ? points : 1;
			fillVBO();
			break;
		case 'h':
			skip *=2;
			skip = skip>MAX_SKIP ? MAX_SKIP : skip; 
			fillVBO();
			break;
		case 'n':
			skip /=2;
			fillVBO();
			break;
		case 'r':
			moveIncrement += moveIncrement>1 ? -1 : 0;
			break;
		case 't':
			moveIncrement += moveIncrement<100000 ? 1 : 0;
			break;
		case 'y':
			counter = 0;
			break;
		case 'u':
			showAll = (showAll+1)%3;
			break;
		case 'i':
			solver = (solver+1)%6;
			fillVBO();			
			break;
		case 'o':
            lineMode = (lineMode+1)%4;
			break;
		case 'p':
			showComponents = (showComponents+1)%3;
			break;
		case ' ':
			running	= !running;
            if(running ) {
                timer(1);
            }
			break;
        case 'm':
            usingVBO = !usingVBO;
            break;
		case '+':
			zoom = zoom * 1.2;
			break;
		case '-':
			zoom = zoom / 1.2;
			break;
        case 'k':
            autorotate +=0.1;
            break;
        case 'l':
            autorotate -=0.1;
            break;
        case ',':
            alpha -= 0.1 ;
            if(alpha<0.1)
                alpha = 0.1;
            fillVBO();
            break;
        case '.':
            alpha += 0.1;
            if(alpha>1.0)
                alpha = 1.0;
            fillVBO();
            break;
		case 27:   // ESC
			exit(0);
	}
	glutPostRedisplay();
}

static int lastx=0;
static int lasty=0;

void mouseClick(int button, int state, int x, int y) {
	lastx=x;
	lasty=y;
}

void mouseMotion(int x, int y) {

	int diffx=x-lastx;
	int diffy=y-lasty;
	lastx=x;
	lasty=y;
	
	rotx += (float) 0.5f * diffy;
	roty += (float) 0.5f * diffx;

	glutPostRedisplay();
}

void reshape(int w, int h) {
	screenWidth = w;
	screenHeight = h;
	
	printf("Reshape: %d/%d\n",w,h);
	
#if 0
	GLfloat fieldOfView = 90.0f;
	glViewport (0, 0, (GLsizei) screenWidth, (GLsizei) screenHeight);
	
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, (GLfloat) screenWidth/(GLfloat) screenHeight, 0.1, 500.0);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
#else
	glViewport(0, 0, screenWidth, screenHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-32, 32, -20, 20, -2000, 2000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
#endif
}


void initGL(int width, int height) {
	
	reshape(width, height);
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	// Vert sync
	int swap_interval = 1;

#ifdef __APPLE__
	CGLContextObj ctx = CGLGetCurrentContext();
	CGLSetParameter(ctx, kCGLCPSwapInterval, &swap_interval);
#endif
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB /*| GLUT_DEPTH*/);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(POSX, POSY);
	glutCreateWindow("Lorenz attractor 3D render by Mario Arias");
	
	// register glut call backs
	glutKeyboardFunc(keyboardDown);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);
	glutReshapeFunc(reshape);
	glutDisplayFunc(draw);  
	//glutIdleFunc(idle);

    timer(1);
	
	keyboardDown('1', 0, 0);	

	initGL(WIDTH, HEIGHT);
	
	glutMainLoop();
	return 0;	
}
