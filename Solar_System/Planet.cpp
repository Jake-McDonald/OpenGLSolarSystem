/************************************************************************************

File: 			Planet.cpp

Author:			Jacob McDonald
A00379749
CSCI 4471
*************************************************************************************/

#include <string>
#include <iostream>
#include "Planet.h"


Planet::Planet(int id, char* name, float dist, float orbitSpeed, float radius, float (&color)[3])
{
	this ->id = id;
	this->name = name;
	distFromSun = dist;
	orbitalSpeed = orbitSpeed;
	this ->radius = radius;
	std::copy(color, color + 3, this ->color);;
}

Planet::Planet()
{
}

Planet::~Planet()
{
}

