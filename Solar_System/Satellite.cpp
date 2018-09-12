/************************************************************************************

File: 			Satellite.cpp

Author:			Jacob McDonald
A00379749
CSCI 4471
*************************************************************************************/

#include "Satellite.h"
#include "freeglut.h"
#include <math.h>

float satOrbit = 180;;
float orbitAngle= 180;
const GLfloat PI = 3.14159;
#define DEG_TO_RAD  PI/180.00

Satellite::Satellite(float xPos, float radius, float primaryAngle, float distFromPlanet)
{
	this->xPos = xPos;
	this->radius = radius;
	this->primaryAngle = primaryAngle;
	this->distFromPlanet = distFromPlanet;
}
Satellite::~Satellite()
{
}

void Satellite::draw()
{
	glColor3f(1, 1, 1);
	satOrbit += 0.6;
	orbitAngle += 5;
	glPushMatrix();
	glRotatef(orbitAngle, 0, 1, 0);
	glTranslatef(distFromPlanet, 0, 0);
	glutSolidSphere(1, 30, 30);
}
