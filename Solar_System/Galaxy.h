#pragma once

struct Position
{
	float x;
	float y;
	float z;
};

void drawPlanets();
void initializeShip();
void drawShip();
void drawCorona();
void drawOrbits();
void drawMoonOrbit(float dist);
void printControls();
void myDisplay(void);
void myIdle();
void specialInput(int key, int x, int y);
void initModels();
void initializeGL(void);
void main(int argc, char** argv);