#include "Time.h"
#include <GLFW\glfw3.h>

Time::Time()
{
	this->deltaTime = 0.0;
	this->lastFrame = glfwGetTime();
}

void Time::StartFrame()
{
	double currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}
