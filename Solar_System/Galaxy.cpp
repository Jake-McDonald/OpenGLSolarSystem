
/************************************************************************************

	File: 			Galaxy.cpp

	Author:			Jacob McDonald
					A00379749
					CSCI 4471
*************************************************************************************/

/* include the library header files */
#include <stdlib.h>
#include <freeglut.h>
#include <iostream>
#include <vector>
#include "Planet.h"
#include <String>
#include <fstream>
#include "Stars.h"
#include "Galaxy.h"
#include "Satellite.h";

//Defines
const GLfloat PI = 3.14159;
#define DEG_TO_RAD  PI/180.00
Stars starField;

int width = 500;
int height = 500;
GLfloat cameraX = 0;
GLfloat cameraY = 0;
GLfloat cameraZ = 0;
GLfloat shipX = 0;
GLfloat shipY = 30;
GLfloat shipZ = -50;
GLfloat zSpeed = 0.5;
GLfloat XYSpeed =  1;
GLfloat theta = 0;
GLfloat thetaSpeed = .1;
GLfloat earthRadius = 2;
GLfloat rotationalModifier = 50;
GLfloat moonDistFromPlanet = 4;

//Scene control variables
bool showStars = false;
bool showOrbits = false;
bool showCorona = false;
bool shieldsUp = false;
bool showSaturnRings = true;

typedef std::vector<GLfloat> vertex;
std::vector<std::string> fileNames = {"enterprise.txt"};
std::vector<vertex> modelVertices;
std::vector<std::vector<int>> triangles;

//Colors
float white[3] = { 1, 1, 1 };
float red[3] = { 1, 0, 0 };
float blue[3] = { 0, 0, 1 };
float yellow[3] = { 1, 1, 0 };
float reddishBrown[3] = { 0.349, 0, 0 };
float paleYellow[3] = { .953, .898, .671 };
float gray[3] = { .745, .745, .745 };
float orange[3] = { .984, .60, .08 };
float paleGold[3] = { .902, .745, .541 };
float paleBlue[3] = { .502, .753, .722 };
float lightBrown[3] = { .651, .482, .357 };

//Quadric objects
GLUquadricObj *disk;
GLUquadricObj *sphere;


std::vector<Planet>  solarSystem = { 
	{0, "Sun", 0, 0, (float)6 , yellow},
	{1, "Mercury", 15, (float)4 * rotationalModifier, (earthRadius * (float).38), gray},
	{2, "Venus", 20, (float)3 * rotationalModifier, (earthRadius * (float).95), paleYellow},
	{3, "Earth", 25, (float)2.8 * rotationalModifier, earthRadius, blue},
	{4, "Mars", 30, (float)2.4 * rotationalModifier, (earthRadius * (float).53), reddishBrown},
	{5, "Jupiter", 35, (float)1.3 * rotationalModifier, (earthRadius * (float)2), orange},
	{6, "Saturn", 40, (float).97 * rotationalModifier, (earthRadius * (float)1.75), paleGold},
	{7, "Uranus", 45, (float).68 * rotationalModifier, (earthRadius * (float)1.5), paleBlue},
	{8, "Neptune", 50, (float).54 * rotationalModifier, (earthRadius * (float)1.1), paleBlue},
	{9, "Pluto", 55, (float).47 * rotationalModifier, (earthRadius * (float).167), red},
};

void drawPlanets()
{
	for (std::vector<Planet>::iterator it = solarSystem.begin(); it != solarSystem.end(); ++it)
	{
		Planet currentPlanet = *it;

		glPushMatrix();
		glColor3fv(currentPlanet.color);
		//glTranslatef(currentPlanet.distFromSun, 0, 0);
		//glTranslatef(-currentPlanet.distFromSun, 0, 0);
		glRotatef(theta * currentPlanet.orbitalSpeed, 0, 1, 0);
		glTranslatef(currentPlanet.distFromSun, 0, 0);
		glutSolidSphere(currentPlanet.radius, 20, 20);

		if (currentPlanet.name == "Earth")
		{
			Satellite moon(currentPlanet.distFromSun, 0.9, theta * currentPlanet.orbitalSpeed, moonDistFromPlanet);

			if(showOrbits) drawMoonOrbit(moonDistFromPlanet);
			moon.draw();
		}
		else if (currentPlanet.name == "Saturn")
		{
			if (showSaturnRings)
			{
				glColor3f(0.93, 0.909, 0.667);
				glRotatef(90, 1, 0, 0);
				gluDisk(disk, 2.8, 3.3, 30, 30);
				gluDisk(disk, 3.3, 4, 30, 30);
				gluDisk(disk, 5, 6, 30, 30);
				gluDisk(disk, 6.5, 7, 30, 30);
			}
		}
		glPopMatrix();
		glPopMatrix();
	}
}

void initializeShip()
{
	std::ifstream infile(fileNames[0]);

	if (!infile.is_open())
	{
		std::cout << "error reading file";
	}
	char type;

	GLfloat x, y, z;

	while (infile >> type >> x >> y >> z)
	{
		if (type == 'v')
		{
			vertex currentVertex = { x, y, z };
			modelVertices.push_back(currentVertex);
		}
		else if (type == 'f')
		{
			std::vector<int> currentTriangle = {(int)x, (int)y, (int)z};
			triangles.push_back(currentTriangle);
		}
	}
}

void drawShip()
{
	for (int i = 0; i < triangles.size(); i++)
	{
		glPushMatrix();
		glTranslatef(shipX, shipY, shipZ);
		glScalef(1.5, 1.5, 0);

		glColor3f((float)i / triangles.size(), (float) i / triangles.size(), (float)i / triangles.size());
		glBegin(GL_TRIANGLE_STRIP);
		std::vector<int> currentTriangle = triangles[i];
		int vertex1 = currentTriangle[0] - 1;
		int vertex2 = currentTriangle[1] - 1;
		int vertex3 = currentTriangle[2] - 1;

		glVertex3f(modelVertices[vertex1][0], modelVertices[vertex1][1], modelVertices[vertex1][2]);
		glVertex3f(modelVertices[vertex2][0], modelVertices[vertex2][1], modelVertices[vertex2][2]);
		glVertex3f(modelVertices[vertex3][0], modelVertices[vertex3][1], modelVertices[vertex3][2]);
		glEnd();

		glPopMatrix();
	}
}

void drawShields()
{
	GLfloat randomBlueValue = (static_cast <GLfloat> (rand()) / (static_cast <float> (RAND_MAX / 1))) + 0.7;
	glColor4f(0, .80, randomBlueValue, 0.3);
	glPushMatrix();
	glTranslatef(shipX, shipY, shipZ);
	glScalef(1, 0.5, 0.5);
	gluSphere(sphere, 0.7, 30, 30);
	glPopMatrix();
}
void drawCorona()
{
	int numLines = 0;
	GLfloat theta = 0;

	while (numLines <= 180)
	{
		GLfloat lineDistance = ((GLfloat)rand() / RAND_MAX + 2) * 5;
		GLfloat endPointX = (lineDistance * sin(DEG_TO_RAD * (theta)));
		GLfloat endPointY = (lineDistance * cos(DEG_TO_RAD * (theta)));
		glLineWidth(3);
		glBegin(GL_LINES);
		glColor3fv(yellow);
		glVertex2f(0, 0);
		glColor4f(1, .549, 0, 0.2);
		glVertex2f(endPointX, endPointY);
		glEnd();

		numLines++;
		theta += 2;
	}
}

void drawOrbits()
{
	glColor3fv(white);
	glLineWidth(1);
	for (int i = 0; i < solarSystem.size(); i++)
	{
		Planet currentPlanet = solarSystem[i];

		float x = currentPlanet.distFromSun;

		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < 361; j++)
		{
			glVertex3f(x * (float)sin(j * DEG_TO_RAD), 0, x * (float)cos(j * DEG_TO_RAD));
		}
		glEnd();
	}
}

void drawMoonOrbit(float dist)
{
	glColor3f(1, 1, 1);
	glLineWidth(1);

	glBegin(GL_LINE_LOOP);
	for (int j = 0; j < 361; j++)
	{
		glVertex3f(dist * (float)sin(j * DEG_TO_RAD), 0, dist * (float)cos(j * DEG_TO_RAD));
	}
	glEnd();
}



/************************************************************************

	Function:		myDisplay

	Description:	Display callback, clears frame buffer and depth buffer,
					positions the camera and draws the cube.

*************************************************************************/
void myDisplay(void)
{

	// clear the screen and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// load the identity matrix into the model view matrix
	glLoadIdentity();

	cameraX = shipX;
	cameraY = shipY ;
	cameraZ = shipZ -2; // place the camera behind the Enterprise

	glTranslatef(0, -2, 0); //shift the scene down so that the Enterprise is near the bottom

	gluLookAt(cameraX, cameraY, cameraZ,
		shipX, shipY, shipZ,
		0, 1, 0);

	if(showStars) starField.draw();
	drawPlanets();
	drawShip();
	if(showOrbits) drawOrbits();
	if(showCorona) drawCorona();
	if(shieldsUp) drawShields();
	glutSwapBuffers();
}


/************************************************************************

	Function:		myIdle

	Description:	Updates the animation when idle.

*************************************************************************/
void myIdle()
{
	theta += thetaSpeed;
	// redraw the new state
	glutPostRedisplay();
}


void specialInput(int key, int x, int y)
{
	GLfloat fraction = 1;
	switch (key)
	{
	case GLUT_KEY_PAGE_UP:
		shipZ += zSpeed;
		break;
	case GLUT_KEY_PAGE_DOWN:
		shipZ -= zSpeed;
		break;
	case GLUT_KEY_LEFT:
		shipX += XYSpeed;
		break;
	case GLUT_KEY_RIGHT:
		shipX -= XYSpeed;
		break;
	case GLUT_KEY_UP:
		shipY += XYSpeed;
		break;
	case GLUT_KEY_DOWN:
		shipY -= XYSpeed;
		break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int xMouse, int yMouse)
{
	switch (key)
	{
	case 'c':
		showCorona = !showCorona;
		break;
	case 'k':
		shieldsUp = !shieldsUp;
		break;
	case 'r':
		showOrbits = !showOrbits;
		break;
	case 's':
		showStars = !showStars;
		break;
	case 't':
		showSaturnRings = !showSaturnRings;
		break;
	}
}

void initModels()
{
	initializeShip();
	starField.initStars(100);
}


/************************************************************************

	Function:		initializeGL

	Description:	Initializes the OpenGL rendering context for display. 

*************************************************************************/
void initializeGL(void)
{
	// enable depth testing
	glEnable(GL_DEPTH_TEST); 

	// set background color to be black
	glClearColor(0,0,0,1.0);
    
	// change into projection mode so that we can change the camera properties
    glMatrixMode(GL_PROJECTION);

	// load the identity matrix into the projection matrix
    glLoadIdentity();

    // set window mode to 3D perspective 
	gluPerspective(110, width / height, 0.1, 1000);
    
    // change into model-view mode so that we can change the object positions
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_LINE_SMOOTH);

	glEnable(GL_BLEND);

	disk = gluNewQuadric();
	gluQuadricNormals(disk, GLU_SMOOTH);
	gluQuadricDrawStyle(disk, GLU_FILL);

	sphere = gluNewQuadric();
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
}
	
 			

/************************************************************************

	Function:		main

	Description:	Sets up the openGL rendering context and the windowing
					system, then begins the display loop.

*************************************************************************/
void main(int argc, char** argv)
{
		// initialize the toolkit
		glutInit(&argc, argv);
		// set display mode
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
		// set window size
		glutInitWindowSize(800, 800);
		// set window position on screen
		glutInitWindowPosition(400, 0);
		// open the screen window
		glutCreateWindow("Solar System");

		// register redraw function
		glutDisplayFunc(myDisplay);
		// register the idle function
		glutIdleFunc(myIdle);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//Register special keys function
		glutSpecialFunc(specialInput);

		glutKeyboardFunc(keyboard);

		//initialize the rendering context
		initializeGL();

		initModels();

		printControls();

		// go into a perpetual loop
		glutMainLoop();
}

void printControls()
{
	std::cout << "\n\n\n" << "Scene Controls\n";
	std::cout << "----------------------------\n\n";
	std::cout << "r:  toggle rings\n";
	std::cout << "s:  toggle stars\n";
	std::cout << "c:  toggle the sun's corona\n";
	std::cout << "k:  toggle shields\n";
	std::cout << "t:  toggle Saturn's rings (default on)\n\n";
	std::cout << "Camera Controls\n";
	std::cout << "----------------------------\n\n";
	std::cout << "Up    Arrow:   move up\n";
	std::cout << "Down  Arrow:   move down\n";
	std::cout << "Right Arrow:   move right\n";
	std::cout << "Left  Arrow:   move left\n";
	std::cout << "PAGE  UP   :   forward\n";
	std::cout << "PAGE DOWN  :   backward\n\n\n";
	std::cout << "Number of vertices found " << modelVertices.size() << "\n";
	std::cout << "Number of faces    found " << triangles.size() << "\n";
}