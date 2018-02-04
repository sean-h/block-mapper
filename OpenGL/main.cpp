#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Window.h"
#include "Input.h"
#include "Scene.h"
#include "Renderer.h"
#include "Physics.h"

void processInput(Input* input);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, 0.0f);
Window* window;
Scene* scene;
Physics* physics;

int main()
{
	glfwInit();

	Input input;
	window = new Window(800, 600);

	Renderer renderer;
	physics = new Physics();
	scene = new Scene();

	while (!window->ShouldClose())
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		input.PollWindowInput(window->GLFWWindow());
		processInput(&input);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.RenderScene(*scene, *window, camera);

		window->EndFrame();
		input.EndFrame();
	}

	glfwTerminate();
	return 0;
}

void processInput(Input* input)
{
	if (input->GetKey(Input::Keys::KEY_ESCAPE))
	{
		window->Close();
	}

	camera.ProcessMouseMovement(input->MouseXDelta(), input->MouseYDelta(), GL_TRUE);

	if (input->GetKey(Input::Keys::KEY_W))
	{
		camera.ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	if (input->GetKey(Input::Keys::KEY_S))
	{
		camera.ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	if (input->GetKey(Input::Keys::KEY_A))
	{
		camera.ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	if (input->GetKey(Input::Keys::KEY_D))
	{
		camera.ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}
	if (input->GetKeyDown(Input::Keys::KEY_F))
	{
		Entity* entity = physics->Raycast(scene, camera.Position, camera.Front, 100.0f);
		if (entity != nullptr)
		{
			std::cout << "Hit: " << entity->ObjectTransform().Position().x << "," << entity->ObjectTransform().Position().y << "," << entity->ObjectTransform().Position().z << std::endl;
		}
	}
}
