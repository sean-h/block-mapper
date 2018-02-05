#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Camera.h"
#include "Window.h"
#include "Input.h"
#include "Scene.h"
#include "Renderer.h"
#include "Physics.h"
#include "DrawBlockTool.h"
#include "Time.h"

int main()
{
	glfwInit();

	Time time;
	Input input;
	Window window(800, 600);
	Renderer renderer;
	Physics physics;
	Scene scene;
	DrawBlockTool activeTool(&scene);

	while (!window.ShouldClose())
	{
		time.StartFrame();

		input.PollWindowInput(window.GLFWWindow());
		if (input.GetKey(Input::Keys::KEY_ESCAPE))
		{
			window.Close();
		}
		scene.Update(&input, &physics, &time);
		activeTool.Update(&scene, &input, &physics);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		renderer.RenderScene(scene, window, *scene.ActiveCamera());

		window.EndFrame();
		input.EndFrame();
	}

	glfwTerminate();
	return 0;
}
