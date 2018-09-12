#pragma once
#include <string>
#include <iostream>

class Planet
{

public:
	//Constructors
	Planet();
	Planet(int id, char* name, float dist, float orbitSpeed, float radius, float (&color)[3]);
	~Planet();

	//Member variables
	int id;
	const char* name;
	float distFromSun;
	float orbitalSpeed;
	float radius;
	float color[3];

private:

};

