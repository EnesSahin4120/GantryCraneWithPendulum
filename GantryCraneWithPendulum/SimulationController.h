#pragma once

#include "Pendulum.h"
#include "Payload.h"
#include "Trolley.h"

#include "CommonValues.h"

class SimulationController
{
public:
	SimulationController();

	void UpdateSimulation(Pendulum &pendulum, Payload &payload, Trolley &trolley, GLfloat deltaTime);

	~SimulationController();

private:
	float previousX;
	float previousVel;
	float angularVelocity;
	float angularDisplacement;
};

