#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
#include "Model.h"
#include "Skybox.h"

#include "Pendulum.h"
#include "Payload.h"
#include "Trolley.h"
#include "SimulationController.h"

GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
uniformSpecularIntensity = 0, uniformShininess = 0,
uniformDirectionalLightTransform = 0, uniformOmniLightPos = 0, uniformFarPlane = 0;

std::vector<Pendulum*> pendulums;
Trolley *trolley;
Payload *payload;
SimulationController *simulationController;
Model gantryCrane;

Window mainWindow;
Mesh* groundMesh;

std::vector<Shader> shaderList;
Shader directionalShadowShader;
Shader omniShadowShader;

Camera camera;
Skybox skybox;

Texture groundTexture;
Texture payloadTexture;
Texture pendulumTexture;
Material material;

DirectionalLight mainLight;
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

unsigned int pointLightCount = 0;
unsigned int spotLightCount = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void CreateObjects()
{
	simulationController = new SimulationController();
	for (int i = 0; i < 4; i++) {
		Pendulum *currentPendulum = new Pendulum(15, 0.0075f, 2.0f, 0.0f);
		pendulums.push_back(currentPendulum);
	}
	trolley = new Trolley(glm::vec3(0.0f, 3.5f, -0.2f), 0.5f);
	payload = new Payload(15, 15, 1.0f, glm::vec3(0, 0, 0));

	unsigned int groundIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat groundVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	1.0f, 0.0f,	    0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 1.0f,	    0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		1.0f, 1.0f,	    0.0f, -1.0f, 0.0f
	};

	groundMesh = new Mesh();
	groundMesh->CreateMesh(groundVertices, groundIndices, 32, 6);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);

	directionalShadowShader.CreateFromFiles("Shaders/directional_shadow_map.vert", "Shaders/directional_shadow_map.frag");
	omniShadowShader.CreateFromFiles("Shaders/omni_shadow_map.vert", "Shaders/omni_shadow_map.geom", "Shaders/omni_shadow_map.frag");
}

void RenderScene()
{
	glm::mat4 model(1.0f);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.05f, 0.0f));
	model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	groundTexture.UseTexture();
	material.UseMaterial(uniformSpecularIntensity, uniformShininess);
	groundMesh->RenderMesh();

	for (int i = 0; i < trolley->connectedPositions.size(); i++) {
		model = glm::mat4(1.0f);
		model = glm::translate(model, trolley->connectedPositions[i]);
		model = glm::rotate(model, glm::radians(pendulums[0]->angleZ), glm::vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(pendulums[0]->angleX), glm::vec3(1, 0, 0));
		model = glm::translate(model, glm::vec3(0.0f, -pendulums[0]->height / 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		pendulumTexture.UseTexture();
		pendulums[0]->mesh->RenderMesh();
	}

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, -0.4f, 0.0f));
	model = glm::scale(model, glm::vec3(0.003f, 0.002f, 0.001f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	gantryCrane.RenderModel();

	model = glm::mat4(1.0f);
	model = glm::translate(model, trolley->centerConnect);
	model = glm::rotate(model, glm::radians(pendulums[0]->angleZ), glm::vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(pendulums[0]->angleX), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0.0f, -pendulums[0]->height, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	payloadTexture.UseTexture();
	payload->mesh->RenderMesh();

	model = glm::mat4(1.0f);
	model = glm::translate(model, trolley->pos);
	model = glm::scale(model, glm::vec3(0.3f, 0.2f, 0.4f));
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
	pendulumTexture.UseTexture();
	trolley->mesh->RenderMesh();
}

void DirectionalShadowMapPass(DirectionalLight* light)
{
	directionalShadowShader.UseShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = directionalShadowShader.GetModelLocation();
	directionalShadowShader.SetDirectionalLightTransform(&light->CalculateLightTransform());

	directionalShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* light)
{
	omniShadowShader.UseShader();

	glViewport(0, 0, light->getShadowMap()->GetShadowWidth(), light->getShadowMap()->GetShadowHeight());

	light->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	uniformModel = omniShadowShader.GetModelLocation();
	uniformOmniLightPos = omniShadowShader.GetOmniLightPosLocation();
	uniformFarPlane = omniShadowShader.GetFarPlaneLocation();

	glUniform3f(uniformOmniLightPos, light->GetPosition().x, light->GetPosition().y, light->GetPosition().z);
	glUniform1f(uniformFarPlane, light->GetFarPlane());
	omniShadowShader.SetLightMatrices(light->CalculateLightTransform());

	omniShadowShader.Validate();

	RenderScene();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderPass(glm::mat4 viewMatrix, glm::mat4 projectionMatrix)
{
	glViewport(0, 0, 1366, 768);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	skybox.DrawSkybox(viewMatrix, projectionMatrix);

	shaderList[0].UseShader();

	uniformModel = shaderList[0].GetModelLocation();
	uniformProjection = shaderList[0].GetProjectionLocation();
	uniformView = shaderList[0].GetViewLocation();
	uniformModel = shaderList[0].GetModelLocation();
	uniformEyePosition = shaderList[0].GetEyePositionLocation();
	uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
	uniformShininess = shaderList[0].GetShininessLocation();

	glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

	shaderList[0].SetDirectionalLight(&mainLight);
	shaderList[0].SetPointLights(pointLights, pointLightCount, 3, 0);
	shaderList[0].SetSpotLights(spotLights, spotLightCount, 3 + pointLightCount, pointLightCount);
	shaderList[0].SetDirectionalLightTransform(&mainLight.CalculateLightTransform());

	mainLight.getShadowMap()->Read(GL_TEXTURE2);
	shaderList[0].SetTexture(1);
	shaderList[0].SetDirectionalShadowMap(2);

	glm::vec3 lowerLight = camera.getCameraPosition();
	lowerLight.y -= 0.3f;
	spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

	shaderList[0].Validate();

	RenderScene();
}

int main()
{
	mainWindow = Window(1366, 768); 
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	payloadTexture = Texture("Textures/Payload.jpg");
	payloadTexture.LoadTexture();
	pendulumTexture = Texture("Textures/pendulumTexture.jpg");
	pendulumTexture.LoadTexture();
	groundTexture = Texture("Textures/sandRoad.jpg");
	groundTexture.LoadTexture();

	material = Material(0.3f, 4);

	gantryCrane = Model();
	gantryCrane.LoadModel("Models/GantryCrane.fbx");

	mainLight = DirectionalLight(2048, 2048,
		1.0f, 1.0f, 1.0f,
		0.1f, 0.9f,
		0.0f, -1.0f, -0.8f);

	pointLights[0] = PointLight(1024, 1024,
		0.01f, 1.5f,
		0.7f, 0.3f, 0.0f,
		0.1f, 0.3f,
		2.0f, 3.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	pointLights[1] = PointLight(1024, 1024,
		0.01f, 1.5f,
		0.7f, 0.3f, 0.0f,
		0.1f, 0.3f,
		-2.0f, 3.0f, 0.0f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;


	spotLights[0] = SpotLight(1024, 1024,
		0.01f, 100.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		2.0f, 0.0f, 0.0f,
		22.5f);
	spotLightCount++;

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/right.jpg");
	skyboxFaces.push_back("Textures/Skybox/left.jpg");
	skyboxFaces.push_back("Textures/Skybox/top.jpg");
	skyboxFaces.push_back("Textures/Skybox/bottom.jpg");
	skyboxFaces.push_back("Textures/Skybox/front.jpg");
	skyboxFaces.push_back("Textures/Skybox/back.jpg");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime(); 
		deltaTime = now - lastTime; 
		lastTime = now;

		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		trolley->UpdatePosition(mainWindow.getsKeys(), deltaTime);
		pendulums[0]->SetAngleX(1.0f, -3.0f, 3.0f, deltaTime);
		simulationController->UpdateSimulation(*pendulums[0], *payload, *trolley, deltaTime);

		DirectionalShadowMapPass(&mainLight);
		for (size_t i = 0; i < pointLightCount; i++)
		{
			OmniShadowMapPass(&pointLights[i]);
		}
		for (size_t i = 0; i < spotLightCount; i++)
		{
			OmniShadowMapPass(&spotLights[i]);
		}
		RenderPass(camera.calculateViewMatrix(), projection);

		mainWindow.swapBuffers();
	}
	return 0;
}