#include "GUIManager.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"
#include "Window.h"
#include "ApplicationContext.h"

GUIManager::GUIManager(Window* window)
{
	ImGui_ImplGlfwGL3_Init(window->GLFWWindow(), true);
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

	ImGui::Render();
}
