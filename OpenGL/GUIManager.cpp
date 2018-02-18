#include "GUIManager.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "Window.h"
#include "ApplicationContext.h"

GUIManager::GUIManager(Window* window)
{
	ImGui_ImplGlfwGL3_Init(window->GLFWWindow(), true);

	windowLocations["Tools"] = GUILocation(10.0f, 10.0f, 150.0f, 200.0f);
	windowLocations["ActiveTool"] = GUILocation(10.0f, 250.0f, 150.0f, 200.0f);
	windowLocations["BlockManager"] = GUILocation(10.0f, 500.0f, 275.0f, 300.0f);
}

GUIManager::~GUIManager()
{
	ImGui_ImplGlfwGL3_Shutdown();
}

void GUIManager::StartFrame()
{
	ImGui_ImplGlfwGL3_NewFrame();
}

void GUIManager::Draw(ApplicationContext* context)
{
	context->ApplicationToolManager()->DrawGUI(context);
	context->ApplicationBlockManager()->DrawGUI(context);
	ImGui::Render();
}

bool GUIManager::MouseOverGUIElement(float mouseX, float mouseY)
{
	for (auto& windowLocation : windowLocations)
	{
		GUILocation winLoc = windowLocation.second;

		if (mouseX >= winLoc.xPosition && mouseX <= winLoc.xPosition + winLoc.width &&
			mouseY >= winLoc.yPosition && mouseY <= winLoc.yPosition + winLoc.height)
		{
			return true;
		}
	}

	return false;
}

GUILocation GUIManager::WindowLocation(std::string windowName)
{
	return windowLocations[windowName];
}

GUILocation::GUILocation()
{
	xPosition = 0.0f;
	yPosition = 0.0f;
	width = 0.0f;
	height = 0.0f;
}
