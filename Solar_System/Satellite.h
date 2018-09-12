#pragma once
class Satellite
{
public:
	Satellite(float xPos, float radius, float primaryAngle, float distFromPlanet);
	~Satellite();
	float xPos;
	float radius;
	float rotationAngle;
	float primaryAngle;
	float distFromPlanet;


	void draw();
	void drawOrbit();
};

