#pragma once

#include <vector>
#include <GL\glew.h>

#include "Mesh.h"
#include "CommonValues.h"

class Payload
{
public:
	Payload();
	Payload(int sectorCount, int stackCount, float radius, glm::vec3 pos);

	glm::vec3 pos;

	GLfloat* vertices;
	unsigned int* indices;
	std::vector<GLfloat> verticesList;
	std::vector<unsigned int> indicesList;

	Mesh* mesh;
	~Payload();

private:
	//Mesh Parameters
	float radius;

	float x, y, z, xy;
	float nx, ny, nz, lengthInv;
	float s, t;

	int k1, k2;

	float sectorStep;
	float stackStep;
	float sectorAngle, stackAngle;
};

