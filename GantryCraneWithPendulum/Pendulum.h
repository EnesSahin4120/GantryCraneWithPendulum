#pragma once

#include <vector>
#include <GL\glew.h>
#include <glm\glm.hpp>
#include "Mesh.h"

#include "CommonValues.h"

class Pendulum
{
public:
	Pendulum();

	Pendulum(int sectorCount, float radius, float height, float angleZ);

	float height;
	float angleZ;
	float angleX;

	void SetAngleX(float speedX, float minXAngle, float maxXAngle, float deltaTime);
	Mesh* mesh;

	~Pendulum();

private:
	std::vector<float> GetUnitCircleVertices();

	int sectorCount;
	float radius;
	float theta;

	void SetVertices();
	void SetIndices();

	std::vector<float> verticesList;
	std::vector<unsigned int> indicesList;
	float* vertices;
	unsigned int* indices;
};

