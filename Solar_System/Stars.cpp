/************************************************************************************

File: 			Stars.cpp

Author:			Jacob McDonald
A00379749
CSCI 4471
*************************************************************************************/

#include "Stars.h"
#include "Galaxy.h"
#include <stdlib.h>
#include <freeglut.h>
#include <vector>
#include <random>

std::vector<Position> stars;
std::default_random_engine rng;

Stars::Stars()
{
}

Stars::~Stars()
{
}

void Stars::initStars(int amount)
{
	rng.seed();
	std::uniform_int_distribution<int> generator(1, 100);
	int numStars = 0;

	while (numStars < amount)
	{
		float randX = generator(rng) * (float)pow(-1, generator(rng));
		float randY = generator(rng) * (float)pow(-1, generator(rng));
		float randZ = generator(rng) * (float)pow(-1, generator(rng));

		Position p;
		p.x = randX;
		p.y = randY;
		p.z = randZ;

		stars.push_back(p);
		numStars++;
	}
}

void Stars::draw()
{
	glBegin(GL_POINTS);
	rng.seed();
	std::uniform_int_distribution<int> generator(0, 1);


	glPointSize(3);

	for (int i = 0; i < stars.size(); i++)
	{
		GLfloat randomRedValue = static_cast <GLfloat> (rand()) / (static_cast <float> (RAND_MAX / 1));
		GLfloat randomGreenValue = static_cast <GLfloat> (rand()) / (static_cast <float> (RAND_MAX / 1));
		GLfloat randomBlueValue = static_cast <GLfloat> (rand()) / (static_cast <float> (RAND_MAX / 1));
		glColor3f(randomRedValue, randomBlueValue, randomGreenValue);
		Position currentStar = stars[i];
		glVertex3f(currentStar.x, currentStar.y, currentStar.z);
	}
	glEnd();
}