#include "Trolley.h"

Trolley::Trolley(glm::vec3 pos, float speed)
{
	this->pos = pos;
	this->speed = speed;

	unsigned int indices[] =
	{
		0, 1, 2,
		0, 2, 3,
		1, 0, 4,
		5, 1, 4,
		2, 1, 5,
		2, 5, 6,
		3, 2, 7,
		2, 6, 7,
		5, 4, 6,
		6, 4, 7,
		0, 3, 4,
		4, 3, 7
	};

	GLfloat vertices[] =
	{
		0, 1, 1, 0, 0, 1, 1, 1,
		1, 1, 1, 0, 0, 1, 1, 1,
		1, 1, 0, 0, 0, 1, 1, 1,
		0, 1, 0, 0, 0, 1, 1, 1,
		0, 0, 1, 0, 0, 1, 1, 1,
		1, 0, 1, 0, 0, 1, 1, 1,
		1, 0, 0, 0, 0, 1, 1, 1,
		0, 0, 0, 0, 0, 1, 1, 1
	};

	mesh = new Mesh();
	mesh->CreateMesh(vertices, indices, 64, 36);

	centerConnect = glm::vec3(pos.x + 0.15f, pos.y, pos.z + 0.2f);
	connectedPositions.push_back(glm::vec3(centerConnect.x + 0.1f, centerConnect.y, centerConnect.z + 0.1f));
	connectedPositions.push_back(glm::vec3(centerConnect.x + 0.1f, centerConnect.y, centerConnect.z - 0.1f));
	connectedPositions.push_back(glm::vec3(centerConnect.x - 0.1f, centerConnect.y, centerConnect.z + 0.1f));
	connectedPositions.push_back(glm::vec3(centerConnect.x - 0.1f, centerConnect.y, centerConnect.z - 0.1f));
}

void Trolley::UpdatePosition(bool* keys, GLfloat deltaTime)
{
	glm::vec3 right = glm::vec3(1, 0, 0);
	float velocity = speed * deltaTime;

	if (keys[GLFW_KEY_RIGHT])
	{
		pos -= right * velocity;
	}
	if (keys[GLFW_KEY_LEFT])
	{
		pos += right * velocity;
	}

	UpdateConnectPositions();
}

void Trolley::UpdateConnectPositions()
{
	centerConnect = glm::vec3(pos.x + 0.15f, pos.y, pos.z + 0.2f);
	connectedPositions[0] = glm::vec3(centerConnect.x + 0.1f, centerConnect.y, centerConnect.z + 0.1f);
	connectedPositions[1] = glm::vec3(centerConnect.x + 0.1f, centerConnect.y, centerConnect.z - 0.1f);
	connectedPositions[2] = glm::vec3(centerConnect.x - 0.1f, centerConnect.y, centerConnect.z + 0.1f);
	connectedPositions[3] = glm::vec3(centerConnect.x - 0.1f, centerConnect.y, centerConnect.z - 0.1f);
}


Trolley::~Trolley()
{
}
