//Riley Taylor CSCI4550 Compass Animation assignment!
//This is very rough. I feel like I could do better, but might need to really look into examples of programming to freshen up using functions for things.

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <iostream>
#include <string>
#include <freeglut.h>
#include <cmath>
#include <ctime>
#include<cstdlib>


#define MSEC_PER_FRAME 10 //1000/10 100FPS
#define PI 3.14159265
#define N 40.0 // Number of vertices on the boundary of the disc.
int Left = 0;
int Right = 0;
double rot = 0;
int rot2 = -rot + 10;
bool pauseMotion = false;
int switchView = 0;
bool PauseMenu = false;
int randomX = 0;
int randomZ = 0;
float RCol1= 0; //random color 1-3
float RCol2=0;
float RCol3=0;
int CAROBJECTS = 20; //number of objects.
bool Collision = false;
int Score = 0; //user score
bool Win = false;
int soundCounter = 0;

float d2r(int degree) { return ((PI / 180.0) * degree); } //degree to radians


class RandomCube { //there should be 20. sigh OOP.. old friend.
public:
	float xValue;
	float yValue;
	float zValue;
	float color1; //maybe random color cubes? why not.
	float color2; //maybe random color cubes? why not.
	float color3; //maybe random color cubes? why not.
	//This below will be collision variables.


public:
	RandomCube() {};
	 //just to create cube.



};

RandomCube CubeArray[20]; //this should not fail lol.

void constructCube() {
	for (int i = 0; i < CAROBJECTS; i++)
	{
		RCol1 =(float) rand() / RAND_MAX; //hahah i got it again.
		RCol2 = (float) rand() / RAND_MAX; //I am a genius!
		RCol3 = (float) rand() / RAND_MAX;
		randomX = (rand() % 12) - 6;
		randomZ = (rand() % 1000);
		CubeArray[i].yValue =2;
		CubeArray[i].xValue = -randomX;
		CubeArray[i].zValue = -randomZ;
		CubeArray[i].color1 = RCol1;
		CubeArray[i].color2 = RCol2;
		CubeArray[i].color3 = RCol3;

		std::cout << CubeArray[i].xValue << std::endl;
		std::cout << CubeArray[i].zValue << std::endl;

	}
}


void writeBitmapString(void* font, const char* string) {
	for (int i = 0; string[i] != '\0'; i++)
		glutBitmapCharacter(font, string[i]);
}

void drawRoad2(void)
{

	glColor3f(0, 0, 0);
	glBegin(GL_LINE_STRIP);
	glVertex3f(-8, 0, 0);
	glVertex3f(-8, 0, -1000);
	glVertex3f(8, 0, -1000);
	glVertex3f(8, 0, 0);
	glEnd();

}

void drawRoadLines()
{

	glColor3f(1, 1, 0);
	glBegin(GL_LINES);
	for (int w = 0; w < 1000;)
	{
		glVertex3f(0, 0.2, -w);
		glVertex3f(0, 0.2, -w - 30);
		w = w + 50;
	}
	glEnd();
}


void drawCar() {
	glPushMatrix();
	glTranslatef(0, 1, -2);
	glScalef(2, 1, 1);
	glColor3f(0, 0, 0); glutWireCube(2);
	glColor3f(1, 0, 0); glutSolidCube(2);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0, 2, 1);
	glColor3f(0, 0, 0); glutWireCube(4);
	glColor3f(.9, 0.9, .9); glutSolidCube(4);
	glPopMatrix();
}
void drawMenu() {
	glColor3f(0, 0, 1);
	glBegin(GL_TRIANGLE_STRIP);
	glVertex3f(-5, 7, -rot + 4.9);
	glVertex3f(0, 2, -rot + 4.9);
	glVertex3f(0, 10, -rot + 4.9);
	glVertex3f(5, 7, -rot + 4.9);

	glEnd();
	
}
void drawCollisonScreen() //this just draws the screen.
{
	glColor3f(0, 0, 0);
	glBegin(GL_POLYGON);
	glVertex3f(-8, 10, -rot+4);
	glVertex3f(-8, 0, -rot+4);
	glVertex3f(8, 0, -rot+4);
	glVertex3f(8, 10, -rot+4);
	glEnd();

	glColor3f(1, 1, 1);
	glRasterPos3f(-1.0, 6, -rot + 5);
	writeBitmapString(GLUT_BITMAP_9_BY_15, "Game Over!");

	glColor3f(1, 1, 1);
	glRasterPos3f(-2.0, 3, -rot + 5);
	writeBitmapString(GLUT_BITMAP_9_BY_15, "Press 'r' to retry!");

}

void drawScore()
{

	glColor3f(0, 0, 0);
	glRasterPos3f(-33.0, 25, -rot -30);
	writeBitmapString(GLUT_BITMAP_9_BY_15, "Score:");
	for (int i = 0; i < CAROBJECTS; i++)
	{
		if (CubeArray[i].zValue == -rot && pauseMotion==false)
			Score = Score + 1;
	}	

	std::string Score2 = std::to_string(Score);
	const char* Fscore = Score2.c_str();
	glRasterPos3f(-25.0, 25, -rot - 30);
	writeBitmapString(GLUT_BITMAP_9_BY_15, Fscore);
}
void drawRandomObjects() {
	for (int i = 0; i < 20; i++)
	{
		glColor3f(CubeArray[i].color1, CubeArray[i].color2, CubeArray[i].color3);
		glPushMatrix();
		glTranslatef(CubeArray[i].xValue, CubeArray[i].yValue, CubeArray[i].zValue);
		glutSolidCube(2);
		glPopMatrix();
	}
}

void CheckCollision()
{
	for (int i = 0; i < CAROBJECTS; i++)
	{ //This is gross but will work
		if ((CubeArray[i].zValue == -rot && CubeArray[i].xValue == Right)
			||(CubeArray[i].zValue == -rot && CubeArray[i].xValue == Right+1)
			||(CubeArray[i].zValue == -rot && CubeArray[i].xValue == Right-1)
			|| (CubeArray[i].zValue == -rot && CubeArray[i].xValue == Right + 2)
			|| (CubeArray[i].zValue == -rot && CubeArray[i].xValue == Right - 2))
		{
			drawCollisonScreen();
			pauseMotion = true;
			Collision = true;
		}

	}
}

void drawFlatGrass() {
	glColor3f(0, 1, 0);
	glBegin(GL_POLYGON);
	glVertex3f(-500,-.01,0);
	glVertex3f(-500, -.01, -1000);
	glVertex3f(500, -.01, -1000);
	glVertex3f(500, -.01, 0);
	glEnd();

}

void DrawBuilding()
{
	glPushMatrix();
	glColor3f(0, .2, .7);
	glScalef(1, 10, 1);
	glTranslatef(-16, 0, -200);
	glutSolidCube(8);
	glPopMatrix();

}
void DrawBuilding1()
{
	glPushMatrix();
	glColor3f(0, .2, .7);
	glScalef(1, 10, 1);
	glTranslatef(16, 0, -700);
	glutSolidCube(8);
	glPopMatrix();

}

void drawWinScreen() //this just draws the screen.
{
	glColor3f(0, .5, 0);
	glBegin(GL_POLYGON);
	glVertex3f(-8, 10, -rot + 4);
	glVertex3f(-8, 0, -rot + 4);
	glVertex3f(8, 0, -rot + 4);
	glVertex3f(8, 10, -rot + 4);
	glEnd();

	glColor3f(1, 1, 1);
	glRasterPos3f(-1.0, 6, -rot + 5);
	writeBitmapString(GLUT_BITMAP_9_BY_15, "You Win!");

	glColor3f(1, 1, 1);
	glRasterPos3f(-3, 3, -rot + 5);
	writeBitmapString(GLUT_BITMAP_9_BY_15, "Press 'r' to exit the Game!");
}

void DrawPavement() {
	glColor3f(.5, .5, .5);
	glBegin(GL_POLYGON);
	glVertex3f(-8, .001, 0);
	glVertex3f(-8, .001, -1000);
	glVertex3f(8, .001, -1000);
	glVertex3f(8, .001, 0);
	glEnd();
}


// Drawing routine.
void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	//------------------------------------------------
	glLoadIdentity();
	gluLookAt(0, 8, (-rot + 10), 0, 5, -rot, 0, 1, 0);
	drawRandomObjects(); //i did it!
	drawRoad2();
	drawFlatGrass();
	drawRoadLines();
	drawScore();
	DrawBuilding();
	DrawBuilding1();
	DrawPavement(); //Gray part of road.
	glPushMatrix();
	glTranslatef(Right, 0, -rot);
	drawCar();
	glPopMatrix();
	CheckCollision();
	if (PauseMenu == true && pauseMotion==true)
	{
		drawMenu();
		glColor3f(1,1, 1);
		glRasterPos3f(-1.0, 6, -rot+5);
		writeBitmapString(GLUT_BITMAP_9_BY_15, "Game Paused");
	}
	if (Collision == true)
	{
		drawCollisonScreen();
	}
	if (-rot == -1000)
	{
		drawWinScreen();
		Win = true;
		if (soundCounter <= 0)
		{
			PlaySound(TEXT("FZX15_Finish!.wav"), NULL, SND_FILENAME | SND_ASYNC);
			soundCounter++;
		}
	}

	//------------------------------------------------
	glDisable(GL_DEPTH_TEST);
	glFlush();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y) {
	switch (key)
	{
	case 27:
		exit(0);
		break;
	case ' ':
		if (pauseMotion)
		{
			pauseMotion = false;
			PauseMenu = false;
			std::cout << "NOT PAUSED\n";
		}
		else {
			int Stop = Right;
			std::cout << Stop;
			Right = Stop;
			pauseMotion = true;
			PauseMenu = true;
			std::cout << " PAUSED\n";
			std::cout << -rot + 5<<"BlackTriangle\n";
			std::cout << -rot + 5.1<<"Text\n";

		}
		break;
	case 'a':
		if (pauseMotion == true)
		{
			break;
		}
		else
		{
			Right = Right - 1;
			if (Right <= -7)
				Right = -6;
			break;
		}
		break;	
	case 'd':
		if (pauseMotion == true)
		{
			break;
		}
		else
		{
			Right = Right + 1;
			if (Right >= 7)
				Right = 6;
			break;
		}
		break;
	case 'r':
		if (Collision == true)
		{
			rot = 0;
			Right = 0;
			pauseMotion = false;
			Score = 0;
			Collision = false;
		}
		if (Win == true)
		{
			exit(0);
		}
		else
		{
			break;
		}
	default:
		break;
	}
}

//Timer function is called in every MSEC_PER_FRAME milliseconds
void timerFunc(int value) {

	if (!pauseMotion) rot = (rot + .5);
	if (rot == 1000)
		pauseMotion = true;

	glutPostRedisplay();
	glutTimerFunc(MSEC_PER_FRAME, timerFunc, value);
}

void specialKeyInput(int key, int x, int y)
{
	if (key == GLUT_KEY_LEFT)
	{
		if (pauseMotion == true)
		{
			
		}
		else
		{
			Right = Right - 1;
			if (Right <= -7)
				Right = -6;
			
		}
		
	}
	if (key == GLUT_KEY_RIGHT)
	{
		if (pauseMotion == true)
		{
		}
		else
		{
			Right = Right + 1;
			if (Right >= 7)
				Right = 6;
		}
	}

	glutPostRedisplay();
}

// Initialization routine.
void setup(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
}

// OpenGL window reshape routine.
void resize(int w, int h) {
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-5.0, 5.0, -5.0, 5.0, 5, 1000);
	glMatrixMode(GL_MODELVIEW);
}

// Main routine.
int main(int argc, char** argv) {
	srand(time(0));
	constructCube();

	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Modelview Transformation Example");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutSpecialFunc(specialKeyInput);

	glutTimerFunc(MSEC_PER_FRAME, timerFunc, 1);

	setup();

	glutMainLoop();
}