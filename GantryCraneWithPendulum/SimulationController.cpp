#include "SimulationController.h"

SimulationController::SimulationController()
{
	previousX = 0.0f;
	previousVel = 0.0f;
	angularVelocity = 0.0f;
	angularDisplacement = 0.0f;
}

void SimulationController::UpdateSimulation(Pendulum &pendulum, Payload &payload, Trolley &trolley, GLfloat deltaTime) {
	float currentX = trolley.pos.x;
	float velocity = (float)((currentX - previousX) / deltaTime);
	velocity *= 25.0f;

	float acceleration = (float)((velocity - previousVel) / deltaTime);
	previousVel = velocity;
	previousX = currentX;

	float angularAcceleration = -(acceleration + (g * (angularDisplacement))) / pendulum.height;

	angularVelocity += (angularAcceleration * deltaTime);
	angularVelocity *= 0.999f;
	angularDisplacement += (angularVelocity * deltaTime);

	pendulum.angleZ = angularDisplacement;
}


SimulationController::~SimulationController()
{
}
