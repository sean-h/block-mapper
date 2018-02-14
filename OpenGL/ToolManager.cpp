#include "ToolManager.h"
#include "DrawBlockTool.h"
#include "SelectTool.h"
#include "imgui.h"

ToolManager::ToolManager(ApplicationContext* context)
{
	activeTool = std::make_unique<SelectTool>(SelectTool(context));
	this->selectedToolButtonIndex = 0;
}

void ToolManager::Update(ApplicationContext * context)
{
	activeTool->Update(context);
}

void ToolManager::DrawGUI(ApplicationContext * context)
{
	bool toolsWindowOpen = true;
	ImGui::Begin("Tools", &toolsWindowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	ImGui::SetWindowPos(ImVec2(10.0f, 10.0f));
	ImGui::SetWindowSize(ImVec2(150.0f, 200.0f));

	ImGui::Text("Tools");

	if (this->ToggleButton("Select", 0, selectedToolButtonIndex))
	{
		activeTool->DisableTool(context);
		activeTool.reset();
		activeTool = std::make_unique<SelectTool>(SelectTool(context));
	}

	if (this->ToggleButton("Draw", 1, selectedToolButtonIndex))
	{
		activeTool->DisableTool(context);
		activeTool.reset();
		activeTool = std::make_unique<DrawBlockTool>(DrawBlockTool(context));
	}

	if (this->ToggleButton("Grid", 2, selectedToolButtonIndex))
	{

	}

	if (this->ToggleButton("Fill", 3, selectedToolButtonIndex))
	{

	}

	ImGui::End();

	activeTool->DrawGUI(context);
}

bool ToolManager::ToggleButton(std::string text, int id, int & selectedButton)
{
	if (id == selectedButton)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.8f, 0.8f));
	}

	bool clicked = ImGui::Button(text.c_str(), ImVec2(-1.0f, 25.0f));

	if (id == selectedButton)
	{
		ImGui::PopStyleColor(3);
	}

	if (clicked)
	{
		selectedButton = id;
	}

	return clicked;
}
