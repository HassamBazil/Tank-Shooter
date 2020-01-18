/*********
CTIS164 - Template Source Program
----------
STUDENT : Hassam Bazil
SECTION : 04
HOMEWORK: 2nd Homework
----------
PROBLEMS: 
*********/

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 700

#define TIMER_PERIOD  10 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false, begin = false, shoot = false;
int  winWidth, winHeight; // current Window width and height
int hx = -150, hy = -350, tx = -600, ty = 0, timer = 100, timerMin = 20, passed = 0, shot=0;
int mx = -420, my = 20, tpoints = 0, points;

typedef struct { //Structure
	int hx, hy;
	bool fwd = true;

}helicopter_t;
helicopter_t heli[5]; //Structure Array for 5 helictopers

void heli_values()
{
	int k;

	for (k = 0; k < 5; k++)
	{
		heli[k].hy = hy - rand() % 400; //generating different heights between the helis

		heli[k].hx = hx + (120 * k);
	}
	
}
						  
void circle(int x, int y, int r)
{
#define PI 3.1415
	float angle;
	glBegin(GL_POLYGON);
	for (int i = 0; i < 100; i++)
	{
		angle = 2 * PI*i / 100;
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
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
		glVertex2f(x + r*cos(angle), y + r*sin(angle));
	}
	glEnd();
}

void print(int x, int y, char *string, void *font)
{
	int len, i;

	glRasterPos2f(x, y);
	len = (int)strlen(string);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, string[i]);
	}
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void *font, char *string, ...)
{
	va_list ap;
	va_start(ap, string);
	char str[1024];
	vsprintf_s(str, string, ap);
	va_end(ap);

	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	for (i = 0; i<len; i++)
	{
		glutBitmapCharacter(font, str[i]);
	}
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, char *string, ...) {
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
	for (i = 0; i<len; i++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
	}
	glPopMatrix();
}

//
// To display onto window using OpenGL commands
//
void scoreboard() { //displaying scoreboard
	glColor3ub(100, 30, 0);
	glRectf(430, 400, 600, -400);
	glRectf(-600, -320, 600, -350);
	glColor3ub(252, 252, 252);
	vprint(-300,-340, GLUT_BITMAP_HELVETICA_18, "<Spacebar> Fire     ||    <F1> Pause/Restart");
	vprint(450, 280, GLUT_BITMAP_HELVETICA_18, "   REMAINING");
	vprint(450, 260, GLUT_BITMAP_HELVETICA_18, "        TIME");
	vprint(450, 230, GLUT_BITMAP_HELVETICA_18, "        %02d:%02d", timerMin,timer);
	vprint(450, 180, GLUT_BITMAP_HELVETICA_18, "   CHOPPERS");
	vprint(450, 160, GLUT_BITMAP_HELVETICA_18, "      MISSED");
	vprint(450, 130, GLUT_BITMAP_HELVETICA_18, "            %d", passed );
	vprint(450, 60, GLUT_BITMAP_HELVETICA_18, "      BURST");
	vprint(450, 40, GLUT_BITMAP_HELVETICA_18, "   CHOPPERS");
	vprint(450, 10, GLUT_BITMAP_HELVETICA_18, "           %d", shot);
	vprint(450, -40, GLUT_BITMAP_HELVETICA_18, "       LAST");
	vprint(450, -60, GLUT_BITMAP_HELVETICA_18, "     POINTS");
	vprint(450, -90, GLUT_BITMAP_HELVETICA_18, "          %d", points);
	vprint(450, -140, GLUT_BITMAP_HELVETICA_18, "     TOTAL");
	vprint(450, -160, GLUT_BITMAP_HELVETICA_18, "    POINTS");
	vprint(450, -190, GLUT_BITMAP_HELVETICA_18, "          %d", tpoints);

}
void tank_display(int tx, int ty) { // displaying tank/weapon
	

	glColor3ub(20, 94, 20);
	glRectf(tx + 20, ty, tx + 120, ty + 10);
	glRectf(tx + 50, ty + 30, tx + 90, ty + 35);
	glColor3ub(12, 58, 12);
	glBegin(GL_QUADS);
	glVertex2f(tx, ty);
	glVertex2f(tx + 140, ty);
	glVertex2f(tx + 110, ty - 35);
	glVertex2f(tx + 30, ty - 35);
	glEnd();

	glRectf(tx + 40, ty+10, tx + 100, ty + 30);

	glLineWidth(5);
	glBegin(GL_LINES);
	glVertex2f(tx + 100, ty + 20);
	glVertex2f(tx + 180, ty + 20);
	glEnd();
}

void missile(int mx, int my) //displaying missile
{
	glColor3ub(3, 17, 3);
	circle(mx, my, 3);
	
}
void helicopter_display(int hx, int hy, int i) // figure of the helicopter
{
	glColor3ub(100, 30, 0);
	glRectf(hx, hy, hx + 30, hy + 26);
	glBegin(GL_TRIANGLES);
	glVertex2f(hx + 10, hy );
	glVertex2f(hx + 20, hy );
	glVertex2f(hx + 15, hy - 35);
	glEnd();
	glColor3ub(66, 16, 22);
	circle(hx + 15, hy + 13, 5);
	glBegin(GL_POLYGON);
	glVertex2f(hx + 5, hy + 26);
	glVertex2f(hx + 5, hy + 45);
	glVertex2f(hx + 8, hy + 50);
	glVertex2f(hx + 20, hy + 50);
	glVertex2f(hx + 23, hy+ 46);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex2f(hx + 5, hy+26);
	glVertex2f(hx + 23, hy+46);
	glVertex2f(hx + 23, hy + 26);
	glEnd();
	glLineWidth(5);
	glColor3ub(66, 16, 22);
	glBegin(GL_LINES);
	glVertex2f(hx+15, hy+13);
	glVertex2f(hx+50, hy+70);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(hx + 15, hy + 13);
	glVertex2f(hx - 20, hy - 44);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(hx + 15, hy + 13);
	glVertex2f(hx + 50, hy - 44);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(hx + 15, hy + 13);
	glVertex2f(hx - 20, hy + 70);
	glEnd();
	
	glRectf(hx+5, hy-40, hx + 25, hy - 30);

}
void displaybase() { //displaying base
	
		glColor3ub(118, 122, 118);
		glRectf(-600, 350, 430, -350);
	
	
}
void display() { //displaying all the things in this function

	
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	displaybase();
	
	tank_display(tx, ty); 


	for (int i = 0; i < 5; i++)
	{
		
		if (begin == true)
			heli[i].hy += 3;
		helicopter_display(heli[i].hx, heli[i].hy, i); 

		if (heli[i].hy >= 350)
		{
			heli[i].hy = hy - rand() % 400;
			passed++;
		}
	}
	if (begin) {
		if (up == true && ty < 350)
		{
			ty += 8;
			if (shoot == false)
				my += 8;
		}
		if (down == true && ty > -350)
		{
			ty -= 8;

			if (shoot == false)
				my -= 8;
		}


		if (shoot)
		{
			missile(mx, my);
			mx += 5;
			if (mx == 500 || mx == tx)
			{
				mx = -420;
				my = ty + 20;
				shoot = false;

			}


		}
	}
	missile(mx, my);
	scoreboard();
	
		
	if (timerMin == 0) // Game over message at the end
	{
		glColor3ub(100, 30, 0);
		glRectf(-150, 50, 120, -50);
		glColor3ub(252, 252, 252);
		vprint(-80, 20, GLUT_BITMAP_HELVETICA_18, "  GAME OVER");
		vprint(-120, 0, GLUT_BITMAP_HELVETICA_18, " Total Choppers Passed: %d", shot + passed);
		vprint(-100, -20, GLUT_BITMAP_HELVETICA_18, "      Total Score: %d", tpoints);
		vprint(-100, -40, GLUT_BITMAP_HELVETICA_18, "    Choppers Hit: %d", shot);
	}

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
	if (key == 32)
		shoot = true;

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
	if (key == GLUT_KEY_F1) // if the user presses F1
	{
		begin = !begin;

		if (timerMin == 0)
		{
			timerMin = 20;
			timer = 100, timerMin = 20, passed = 0, shot = 0;
			tpoints = 0, points = 0;
			for (int k = 0; k < 5; k++)
			{
				heli[k].hy = hy - rand() % 400;

				heli[k].hx = hx + (120 * k);
			}
			ty = 0;
			mx = -420, my = 20;
		}

	}
	switch (key) {
	case GLUT_KEY_UP: up = true; break;
	case GLUT_KEY_DOWN: down = true; break;
	case GLUT_KEY_LEFT: left = true; break;
	case GLUT_KEY_RIGHT: right = true; break;
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
	// Write your codes here.



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



	// to refresh the window it calls display() function
	glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {
	if (begin == true)
	{
		
		timer--;
		if (timer % 100 == 0)
		{
			timerMin--;
			timer = 100;
		}
		else if (timerMin == 0)
		{
			timer = 0;
			begin = false;
			
		}
		
		
	}
	
	
	for (int i = 0; i < 5; i++)
	{
		if ((mx >= heli[i].hx - 20 && mx <= heli[i].hx + 50) && (my >= heli[i].hy - 70 && my <= heli[i].hy + 44))
		{
			
			if (my >= heli[i].hy - 26 && my < heli[i].hy - 4)
			{
				tpoints += 5; points = 5;
			}
			else if ((my >= heli[i].hy - 37 && my < heli[i].hy - 26) || (my > heli[i].hy - 4 && my <= heli[i].hy + 7))
			{
				tpoints += 4; points = 4;
			}
			else if ((my >= heli[i].hy - 48 && my < heli[i].hy - 37) || (my > heli[i].hy + 7 && my <= heli[i].hy + 18))
			{
				tpoints += 3;  points = 3;
			}
			else if ((my >= heli[i].hy - 59 && my < heli[i].hy - 48) || (my > heli[i].hy + 19 && my <= heli[i].hy + 29))
			{
				tpoints += 2; points = 3;
			}
			else if ((my >= heli[i].hy - 70 && my < heli[i].hy - 59) || (my > heli[i].hy + 29 && my <= heli[i].hy + 40))
			{
				tpoints += 1; points = 1;
			}
			heli[i].hy = hy - rand() % 400; shot++;
		}
	}
	glutTimerFunc(TIMER_PERIOD, onTimer, 0);
	// Write your codes here.

	// to refresh the window it calls display() function
	glutPostRedisplay(); // display()

}
#endif

void Init() {

	// Smoothing shapes
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}



void main(int argc, char *argv[]) {

	heli_values();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Bazilater Shooter");

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

	Init();
	srand(time(NULL));
	glutMainLoop();
}