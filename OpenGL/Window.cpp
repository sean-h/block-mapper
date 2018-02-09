#include "Window.h"
#include <iostream>

Input* callbackInput;

Window::Window(int width, int height)
	: width(width), height(height)
{
	this->open = true;

	SetWindowHints();
	
	std::unique_ptr<GLFWwindow, DestroyglfwWin> window(glfwCreateWindow(this->width, this->height, "OpenGL", NULL, NULL));
	
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		throw;
	}

	glfwWindow = std::move(window);
	glfwMakeContextCurrent(glfwWindow.get());
	glfwSetInputMode(glfwWindow.get(), GLFW_STICKY_KEYS, 1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		throw;
	}

	glViewport(0, 0, this->width, this->height);
	glfwSetFramebufferSizeCallback(glfwWindow.get(), OnFrameBufferSizeChanged);
	glfwSetScrollCallback(glfwWindow.get(), scroll_callback);

	SetOpenGLCapabilities();
}

void Window::EndFrame()
{
	glfwSwapBuffers(glfwWindow.get());
	glfwPollEvents();

	if (glfwWindowShouldClose(glfwWindow.get()))
	{
		open = false;
	}
}

void Window::Close()
{
	open = false;
}

void Window::SetCallbackInput(Input * input)
{
	callbackInput = input;
}

void Window::SetWindowHints()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
}

void Window::SetOpenGLCapabilities()
{
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OnFrameBufferSizeChanged(GLFWwindow * window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (callbackInput != nullptr)
	{
		callbackInput->SetScrollWheel(yoffset);
	}
}