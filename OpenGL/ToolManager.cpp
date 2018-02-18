#include "ToolManager.h"
#include "DrawBlockTool.h"
#include "SelectBlockTool.h"
#include "GridBlockTool.h"
#include "MoveBlockTool.h"
#include "imgui.h"

ToolManager::ToolManager(ApplicationContext* context)
{
	activeTool = std::make_unique<DrawBlockTool>(DrawBlockTool(context));
	this->selectedToolButtonIndex = 1;
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
		this->SelectTool(context, 0);
	}

	if (this->ToggleButton("Move", 1, selectedToolButtonIndex))
	{
		this->SelectTool(context, 1);
	}

	if (this->ToggleButton("Draw", 2, selectedToolButtonIndex))
	{
		this->SelectTool(context, 2);
	}

	if (this->ToggleButton("Grid", 3, selectedToolButtonIndex))
	{
		this->SelectTool(context, 3);
	}

	if (this->ToggleButton("Fill", 4, selectedToolButtonIndex))
	{
	}

	ImGui::End();

	bool activeToolWindowOpen = true;
	ImGui::Begin(activeTool->Name().c_str(), &activeToolWindowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	ImGui::SetWindowPos(ImVec2(10.0f, 250.0f));
	ImGui::SetWindowSize(ImVec2(150.0f, 200.0f));
	activeTool->DrawGUI(context);
	ImGui::End();
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

void ToolManager::SelectTool(ApplicationContext* context, int toolIndex)
{
	activeTool->DisableTool(context);
	activeTool.reset();

	switch (toolIndex)
	{
	case 0:
		activeTool = std::make_unique<SelectBlockTool>(SelectBlockTool(context));
		break;
	case 1:
		activeTool = std::make_unique<MoveBlockTool>(MoveBlockTool(context));
		break;
	case 2:
		activeTool = std::make_unique<DrawBlockTool>(DrawBlockTool(context));
		break;
	case 3:
		activeTool = std::make_unique<GridBlockTool>(GridBlockTool(context));
		break;
	case 4:
		break;
	default:
		break;
	}
}
