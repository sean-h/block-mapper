#include <glad\glad.h>
#include <GLFW\glfw3.h>
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

		if (!window->IsWindowMinimized())
		{
			input->PollWindowInput(window->GLFWWindow());
			input->MouseOverGUIElement(applicationContext.ApplicationGUIManager()->MouseOverGUIElement(input->MouseX(), input->MouseY()));

			Scene* scene = applicationContext.ApplicationScene();
			scene->Update(&applicationContext);
			applicationContext.ApplicationBlockManager()->Update(&applicationContext);
			applicationContext.ApplicationToolManager()->Update(&applicationContext);

			applicationContext.ApplicationGUIManager()->StartFrame();

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			applicationContext.ApplicationRenderer()->RenderScene(&applicationContext);
			applicationContext.ApplicationDebug()->Render(&applicationContext);
			applicationContext.ApplicationGUIManager()->Draw(&applicationContext);
		}

		window->EndFrame();
		input->EndFrame();
		applicationContext.ApplicationDebug()->EndFrame(&applicationContext);
	}

	glfwTerminate();
	return 0;
}
