#pragma once

#include <vector>

#include <GL\glew.h>
#include <glm\glm.hpp>
#include <GLFW\glfw3.h>

#include "Mesh.h"

class Trolley
{
public:
	Trolley(glm::vec3 pos, float speed);

	glm::vec3 pos;
	glm::vec3 centerConnect;
	std::vector<glm::vec3> connectedPositions;

	void UpdatePosition(bool* keys, GLfloat deltaTime);

	Mesh* mesh;
	
	~Trolley();

private:
	float speed;
	void UpdateConnectPositions();
};

