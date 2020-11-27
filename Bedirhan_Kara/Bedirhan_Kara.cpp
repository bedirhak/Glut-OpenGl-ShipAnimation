/*********
   CTIS164 - Template Source Program
----------
STUDENT : Bedirhan Kara
SECTION : 3
HOMEWORK: 1
----------
PROBLEMS: Nothing
----------
ADDITIONAL FEATURES: When you click on a moving ship, it accelerates and if you click same ship again, it returns its previous pace. Also road start to move just when ships start to move and stop when they stop.
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  20 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

int xB = 0, yB = 0; //initial position
int currentPosOfX, currentPosOfY;

int numOfShape;
bool move = 0;
bool flag = 0; // To restart the number of shap count

int a, b;
int xBr = 1400;

typedef struct {
	int numX, numY, pace;
	bool active, clicked=0;
}ship_t;

ship_t ship[10];

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
	float angle;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r * cos(angle), y + r * sin(angle));
	}
	glEnd();
}

void print(int x, int y, const char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, const char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char *string, ...) {
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(size, size, 1);

	int len, i;
	len = (int)strlen(str);
	for (i = 0; i < len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

bool onTheShip(int x, int y, int currentX, int currentY, int radius) 
{
	float span = sqrt((x - currentX)*(x - currentX) + (y - currentY) * (y - currentY));

	if (span < radius)
		return true;
	else
		return false;
}

void drawShip(int xB, int yB)
{
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex2f(-60 + xB, 0 + yB);
	glVertex2f(60 + xB, 0 + yB);
	glVertex2f(60 + xB, 40 + yB);
	glVertex2f(-60 + xB, 40 + yB);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex2f(-60 + xB, 70 + yB);
	glVertex2f(60 + xB, 70 + yB);
	glVertex2f(60 + xB, 40 + yB);
	glVertex2f(-60 + xB, 40 + yB);
	glEnd();

	glLineWidth(2.2);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(60 + xB, 40 + yB);
	glVertex2f(-60 + xB, 40 + yB);
	glEnd();
	
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex2f(-40 + xB, 70 + yB);
	glVertex2f(60 + xB, 70 + yB);
	glVertex2f(60 + xB, 90 + yB);
	glVertex2f(-40 + xB, 90 + yB);
	glEnd();

	// top of the flue
	glColor3f(1, 0, 0);
	circle(-50 + xB, 105 + yB, 10);

	glColor3f(1, 1, 1);
	circle(-50 + xB, 103 + yB, 5);

	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex2f(-60 + xB, 70 + yB);
	glVertex2f(-60 + xB, 105 + yB);
	glVertex2f(-40 + xB, 105 + yB);
	glVertex2f(-40 + xB, 70 + yB);
	glEnd();

	glLineWidth(2.2);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex2f(-60 + xB, 70 + yB);
	glVertex2f(60 + xB, 70 + yB);
	glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(60 + xB, 40 + yB);
	glVertex2f(90 + xB, 40 + yB);
	glVertex2f(60 + xB, 0 + yB);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(-60 + xB, 40 + yB);
	glVertex2f(-90 + xB, 40 + yB);
	glVertex2f(-60 + xB, 0 + yB);
	glEnd();

	// windowses
	glColor3f(0, 0, 1);
	circle(-30 + xB, 20 + yB, 10);
	circle(0 + xB, 20 + yB, 10);
	circle(30 + xB, 20 + yB, 10);

	// windowses' inner part
	glColor3f(1, 1, 1);
	circle(-30 + xB, 18 + yB, 4);
	circle(0 + xB, 18 + yB, 4);
	circle(30 + xB, 18 + yB, 4);

	// top windowses
	glColor3f(0, 0, 1);
	circle(-40 + xB, 55 + yB, 5);
	circle(-20 + xB, 55 + yB, 5);
	circle(0 + xB, 55 + yB, 5);
	circle(20 + xB, 55 + yB, 5);
	circle(40 + xB, 55 + yB, 5);

	// part of lower triangle
	glBegin(GL_POLYGON);
	glColor3f(1, 1, 1);
	glVertex2f(-60 + xB, 32 + yB);
	glVertex2f(-60 + xB, 37 + yB);
	glVertex2f(60 + xB, 37 + yB);
	glVertex2f(60 + xB, 32 + yB);
	glEnd();
}

void drawCar(int xB, int yB)
{
	// WHEELS
	glColor3f(0.431, 0.411, 0.447);
	circle(-45 + xB, -11 + yB, 15);
	circle(35 + xB, -11 + yB, 15);

	glColor3f(0.768, 0.733, 0.733);
	circle(35 + xB, -9 + yB, 7);
	circle(-45 + xB, -9 + yB, 7);

	// headLighter
	glColor3f(0.952, 0.960, 0.219);
	circle(-72 + xB, 25 + yB, 5);

	glColor3f(0.776, 0.650, 0.886);
	glBegin(GL_POLYGON);
	glVertex2f(-70 + xB, 30 + yB);
	glVertex2f(-70 + xB, 0 + yB);
	glVertex2f(70 + xB, 0 + yB);
	glVertex2f(70 + xB, 30 + yB);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(-40 + xB, 30 + yB);
	glVertex2f(-40 + xB, 70 + yB);
	glVertex2f(40 + xB, 70 + yB);
	glVertex2f(40 + xB, 30 + yB);
	glEnd();

	glColor3f(1, 1, 1);
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(-40 + xB, 0 + yB);
	glVertex2f(-40 + xB, 30 + yB);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(40 + xB, 0 + yB);
	glVertex2f(40 + xB, 30 + yB);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(0 + xB, 0 + yB);
	glVertex2f(0 + xB, 70 + yB);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(-70 + xB, 30 + yB);
	glVertex2f(70 + xB, 30 + yB);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(-40 + xB, 30 + yB);
	glVertex2f(-40 + xB, 70 + yB);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(-40 + xB, 30 + yB);
	glVertex2f(-40 + xB, 70 + yB);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(40 + xB, 70 + yB);
	glVertex2f(-40 + xB, 70 + yB);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(40 + xB, 70 + yB);
	glVertex2f(40 + xB, 30 + yB);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(70 + xB, 30 + yB);
	glVertex2f(70 + xB, 0 + yB);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(-70 + xB, 0 + yB);
	glVertex2f(70 + xB, 0 + yB);
	glEnd();

	glBegin(GL_LINES);
	glVertex2f(-70 + xB, 0 + yB);
	glVertex2f(-70 + xB, 30 + yB);
	glEnd();
	
	glColor3f(0.988, 0.960, 0.811);
	glBegin(GL_POLYGON);
	glVertex2f(-35 + xB, 35 + yB);
	glVertex2f(-35 + xB, 65 + yB);
	glVertex2f(-5 + xB, 65 + yB);
	glVertex2f(-5 + xB, 35 + yB);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(35 + xB, 35 + yB);
	glVertex2f(35 + xB, 65 + yB);
	glVertex2f(5 + xB, 65 + yB);
	glVertex2f(5 + xB, 35 + yB);
	glEnd();

	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex2f(-20 + xB, 20 + yB);
	glVertex2f(-20 + xB, 25 + yB);
	glVertex2f(-10 + xB, 25 + yB);
	glVertex2f(-10 + xB, 20 + yB);
	glEnd();

	glBegin(GL_POLYGON);
	glVertex2f(20 + xB, 20 + yB);
	glVertex2f(20 + xB, 25 + yB);
	glVertex2f(30 + xB, 25 + yB);
	glVertex2f(30 + xB, 20 + yB);
	glEnd();
}

void drawStar(int xB, int yB) // really small star
{
	glColor3f(0.941, 0.960, 0.196);
	glBegin(GL_TRIANGLES);
	glVertex2f(-643 + xB, 346 + yB);
	glVertex2f(-638 + xB, 350 + yB);
	glVertex2f(-638 + xB, 359 + yB);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(-646 + xB, 353 + yB);
	glVertex2f(-638 + xB, 350 + yB);
	glVertex2f(-630 + xB, 353 + yB);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(-633 + xB, 346 + yB);
	glVertex2f(-638 + xB, 350 + yB);
	glVertex2f(-638 + xB, 359 + yB);
	glEnd();

}

void drawBackground()
{
	// STARS IN THE SKY
	for (int k = 0; k < 15; k++)
	{
		a = rand() % 1400;
		b = rand() % 400 - 160;
		drawStar(a, b);
	}

	//BEACH
	glBegin(GL_POLYGON);
	glColor3f(0.941, 0.627, 0.219);
	glVertex2f(700 + xB, -400 + yB);
	glVertex2f(700 + xB, -28 + yB);
	glVertex2f(-700 + xB, -28 + yB);
	glVertex2f(-700 + xB, -400 + yB);
	glEnd();

	//SEA
	glBegin(GL_POLYGON);
	glColor3f(0.105, 0.062, 0.717);
	glVertex2f(-700 + xB, 160 + yB);
	glVertex2f(-700 + xB, -20 + yB);
	glVertex2f(700 + xB, -20 + yB);
	glVertex2f(700 + xB, 160 + yB);
	glEnd();
	
	// SEA's CIRCLES
	glColor3f(0.105, 0.062, 0.717);
	circle(-616 + xB, 15 + yB, 136);

	glColor3f(0.105, 0.062, 0.717);
	circle(-504 + xB, 15 + yB, 95);

	glColor3f(0.105, 0.062, 0.717);
	circle(-393 + xB, -43 + yB, 40);

	glColor3f(0.105, 0.062, 0.717);
	circle(-337 + xB, -33 + yB, 25);

	glColor3f(0.105, 0.062, 0.717);
	circle(-222 + xB, -10 + yB, 93);

	glColor3f(0.105, 0.062, 0.717);
	circle(-109 + xB, -42 + yB, 37);

	glColor3f(0.105, 0.062, 0.717);
	circle(-13 + xB, 47 + yB, 93);

	glColor3f(0.105, 0.062, 0.717);
	circle(66 + xB, -28 + yB, 60);

	glColor3f(0.105, 0.062, 0.717);
	circle(136 + xB, -31 + yB, 39);

	glColor3f(0.105, 0.062, 0.717);
	circle(682 + xB, 40 + yB, 120);

	glColor3f(0.105, 0.062, 0.717);
	circle(209 + xB, 3 + yB, 59);

	glColor3f(0.105, 0.062, 0.717);
	circle(268 + xB, 10 + yB, 93);

	glColor3f(0.105, 0.062, 0.717);
	circle(358 + xB, -21 + yB, 25);

	glColor3f(0.105, 0.062, 0.717);
	circle(462 + xB, -10 + yB, 73);

	glColor3f(0.105, 0.062, 0.717);
	circle(560 + xB, -34 + yB, 33);

	glColor3f(0.105, 0.062, 0.717);
	circle(387 + xB, -19 + yB, 10);

	glColor3f(0.105, 0.062, 0.717);
	circle(-66 + xB, -36 + yB, 23);

	//MOON
	glColor3f(0.564, 0.576, 0.541);
	circle(476 + xB, 320 + yB, 112);
	
	// Say Hello to Idlib's martyrs
	glBegin(GL_POLYGON);
	glColor3f(1, 0, 0);
	glVertex2f(416 + xB, 363 + yB);
	glVertex2f(416 + xB, 323 + yB);
	glVertex2f(485 + xB, 323 + yB);
	glVertex2f(485 + xB, 363 + yB);
	glEnd();

	glColor3f(1, 1, 1);
	circle(436 + xB, 343 + yB, 15);

	glColor3f(1, 0, 0);
	circle(442 + xB, 343 + yB, 12);

	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(0, 0, 0);
	glVertex2f(416 + xB, 263 + yB);
	glVertex2f(416 + xB, 363 + yB);
	glEnd();

	glColor3f(1, 1, 1);

	glBegin(GL_TRIANGLES);
	glVertex2f(457 + xB, 341 + yB);
	glVertex2f(447 + xB, 346 + yB);
	glVertex2f(466 + xB, 346 + yB);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(457 + xB, 341 + yB);
	glVertex2f(457 + xB, 352 + yB);
	glVertex2f(450 + xB, 336 + yB);
	glEnd();

	glBegin(GL_TRIANGLES);
	glVertex2f(457 + xB, 352 + yB);
	glVertex2f(457 + xB, 341 + yB);
	glVertex2f(464 + xB, 336 + yB);
	glEnd();

	glBegin(GL_POLYGON);
	glColor3f(0.282, 0.286, 0.274);
	glVertex2f(700 + xB, -212 + yB);
	glVertex2f(700 + xB, -371 + yB);
	glVertex2f(-700 + xB, -371 + yB);
	glVertex2f(-700 + xB, -212 + yB);
	glEnd();

	// lines of the road
	glColor3f(1, 1, 1);
	glLineWidth(4);
	glBegin(GL_LINES);

	glVertex2f(-740 + xBr, -288 + yB);
	glVertex2f(-660 + xBr, -288 + yB);

	glVertex2f(-620 + xBr, -288 + yB);
	glVertex2f(-540 + xBr, -288 + yB);

	glVertex2f(-500 + xBr, -288 + yB);
	glVertex2f(-420 + xBr, -288 + yB);

	glVertex2f(-380 + xBr, -288 + yB);
	glVertex2f(-300 + xBr, -288 + yB);

	glVertex2f(-260 + xBr, -288 + yB);
	glVertex2f(-180 + xBr, -288 + yB);

	glVertex2f(-140 + xBr, -288 + yB);
	glVertex2f(-60 + xBr, -288 + yB);

	glVertex2f(-20 + xBr, -288 + yB);
	glVertex2f(60 + xBr, -288 + yB);

	glVertex2f(100 + xBr, -288 + yB);
	glVertex2f(180 + xBr, -288 + yB);

	glVertex2f(220 + xBr, -288 + yB);
	glVertex2f(300 + xBr, -288 + yB);

	glVertex2f(340 + xBr, -288 + yB);
	glVertex2f(420 + xBr, -288 + yB);

	glVertex2f(460 + xBr, -288 + yB);
	glVertex2f(540 + xBr, -288 + yB);

	glVertex2f(580 + xBr, -288 + yB);
	glVertex2f(660 + xBr, -288 + yB);

	glVertex2f(700 + xBr, -288 + yB);
	glVertex2f(780 + xBr, -288 + yB);
	glEnd();

	// side lines of the road
	glLineWidth(4);
	glBegin(GL_LINES);
	glVertex2f(-700 + xB, -220 + yB);
	glVertex2f(700 + xB, -220 + yB);

	glVertex2f(-700 + xB, -365 + yB);
	glVertex2f(700 + xB, -365 + yB);
	glEnd();

	drawCar(-600, -330);
	drawCar(-280, -306);
	drawCar(240, -276);
}

//
// To display onto window using OpenGL commands
//
void display() {
	//
	// clear window to black
	//
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	drawBackground();
	//drawSatellite(0, 0);

	for (int j = 0; j < numOfShape; j++)
	{
		if (ship[j].active)
		{
			drawShip(ship[j].numX, ship[j].numY);
		}
	}

	glBegin(GL_QUADS);
	glColor4f(0.525, 0.964, 0.487, 0.3);
	glVertex2f(-98, 378);
	glVertex2f(-98, 340);
	glVertex2f(31, 340);
	glVertex2f(31, 378);
	glEnd();

	glColor3f(0.725, 0.844, 0.362);
	vprint(-85, 365, GLUT_BITMAP_8_BY_13, "Bedirhan Kara");
	vprint(-65, 345, GLUT_BITMAP_8_BY_13, "21702828");

	if (numOfShape > 0 && !move)
	{
		glColor3ub(0, 0, 0);
		vprint(500, -387, GLUT_BITMAP_8_BY_13, "F1 - For moving ships");
	}

	if (numOfShape < 10 && !move)
	{
		glColor3ub(0, 0, 0);
		vprint(-660, -387, GLUT_BITMAP_8_BY_13, "Click on the screen to create objects (maximum 10 objects)");
	}

	//vprint(0, 0, GLUT_BITMAP_8_BY_13, "(%d,%d)",currentPosOfX,currentPosOfY);

	glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
	// exit when ESC is pressed.
	if (key == 27)
		exit(0);

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
	}

	if (key == GLUT_KEY_F1)
	{
		move = 1;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
	// Write your codes here.
	switch (key) {
	case GLUT_KEY_UP: up = false; break;
	case GLUT_KEY_DOWN: down = false; break;
	case GLUT_KEY_LEFT: left = false; break;
	case GLUT_KEY_RIGHT: right = false; break;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && numOfShape < 10 && stat == GLUT_DOWN && (winHeight / 2 - y)>-61 && (winHeight / 2 - y) < 160 && !move)
	{
			ship[numOfShape].numX = x - winWidth / 2;
			ship[numOfShape].numY = winHeight / 2 - y;
			ship[numOfShape].active = 1;
			numOfShape++;
	}

	if (button == GLUT_RIGHT_BUTTON && stat == GLUT_DOWN)
		for (int i = 0; i < numOfShape; i++)
			if (onTheShip(ship[i].numX, ship[i].numY, currentPosOfX, currentPosOfY-45, 45))
				ship[i].clicked = !ship[i].clicked;
		
	// to refresh the window it calls display() function
	glutPostRedisplay();
}
//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
	winWidth = w;
	winHeight = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

void onMoveDown(int x, int y) {
	// Write your codes here.

	// to refresh the window it calls display() function   
	glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
	// Write your codes here.
	currentPosOfX = x - winWidth / 2;
	currentPosOfY = winHeight / 2 - y;

	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.
	if (move) {
		flag = 1;
		for (int j = 0; j < numOfShape; j++) 
		{
			ship[j].numX+=ship[j].pace;
		}
	}

	for (int j = 0; j < numOfShape; j++) {
		if (ship[j].numX < 790)
			flag = 0;
	}

	if (flag) {
		numOfShape = 0;
		move = 0;

		for(int bedirhan=0;bedirhan<10;bedirhan++)
		{
			ship[bedirhan].clicked = 0;
		}

	}

	// to moves lines of the road while ships are moving on the sea
	
	if (xBr >= 40 + 40)
	{
		xBr = -(0 + 40);
	}

	if (move)
		xBr++;

	//to increase the pace of ship

	for (int k = 0; k < 10; k++)
	{
		if (ship[k].clicked)
			ship[k].pace = 5;
		else
			ship[k].pace = 1;
	}

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()
}
#endif

void Init()
{
	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void main(int argc, char *argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//glutInitWindowPosition(100, 100);
	glutCreateWindow("HW-1: Animating Complex Shapes (by Bedirhan KARA)");
	glutDisplayFunc(display);
	glutReshapeFunc(onResize);
	//
	// keyboard registration
	//
	glutKeyboardFunc(onKeyDown);
	glutSpecialFunc(onSpecialKeyDown);
	glutKeyboardUpFunc(onKeyUp);
	glutSpecialUpFunc(onSpecialKeyUp);
	//
	// mouse registration
	//
	glutMouseFunc(onClick);
	glutMotionFunc(onMoveDown);
	glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
	// timer event
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif
	//initialize random generator
	srand(time(NULL));

	Init();

	glutMainLoop();
}