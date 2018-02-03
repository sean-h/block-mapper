#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <memory>
#include <unordered_map>
#include "Input.h"

struct DestroyglfwWin {
	void operator()(GLFWwindow* ptr) {
		glfwDestroyWindow(ptr);
	}
};

class Window
{
public:
	Window(int width, int height);

	int Width() const { return width; }
	int Height() const { return height; }
	GLFWwindow* GLFWWindow() const { return glfwWindow.get(); }
	bool ShouldClose() const { return !open; }
	void EndFrame();
	void Close();

private:
	int width;
	int height;
	bool open;
	std::unique_ptr<GLFWwindow, DestroyglfwWin> glfwWindow;

	void SetWindowHints();
	void SetOpenGLCapabilities();
};

void OnFrameBufferSizeChanged(GLFWwindow * window, int width, int height);