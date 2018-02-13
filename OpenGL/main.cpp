#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "Camera.h"
#include "DrawBlockTool.h"
#include "ApplicationContext.h"

int main()
{
	glfwInit();

	ApplicationContext applicationContext;

	while (!applicationContext.ApplicationWindow()->ShouldClose())
	{
		applicationContext.ApplicationTime()->StartFrame();

		Window* window = applicationContext.ApplicationWindow();
		Input* input = applicationContext.ApplicationInput();
		input->PollWindowInput(window->GLFWWindow());
		if (input->GetKey(Input::Keys::KEY_ESCAPE))
		{
			window->Close();
		}

		Scene* scene = applicationContext.ApplicationScene();
		scene->Update(&applicationContext);
		applicationContext.ApplicationToolManager()->Update(&applicationContext);

		applicationContext.ApplicationGUIManager()->StartFrame();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		applicationContext.ApplicationRenderer()->RenderScene(&applicationContext);
		applicationContext.ApplicationGUIManager()->Draw(&applicationContext);

		window->EndFrame();
		input->EndFrame();
	}

	glfwTerminate();
	return 0;
}
