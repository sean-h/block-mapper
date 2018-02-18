#include "ToolManager.h"
#include "DrawBlockTool.h"
#include "SelectBlockTool.h"
#include "GridBlockTool.h"
#include "MoveBlockTool.h"
#include "ApplicationContext.h"
#include "imgui.h"

ToolManager::ToolManager(ApplicationContext* context)
{
	activeTool = std::make_unique<DrawBlockTool>(DrawBlockTool(context));
	this->selectedToolButtonIndex = 2;
}

void ToolManager::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();

	if (input->GetKeyDown(Input::Keys::KEY_1))
	{
		this->SelectTool(context, 0);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_2))
	{
		this->SelectTool(context, 1);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_3))
	{
		this->SelectTool(context, 2);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_4))
	{
		this->SelectTool(context, 3);
	}

	if (activeTool)
	{
		activeTool->Update(context);
	}
}

void ToolManager::DrawGUI(ApplicationContext * context)
{
	bool toolsWindowOpen = true;
	ImGui::Begin("Tools", &toolsWindowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	GUILocation toolsWindowLocation = context->ApplicationGUIManager()->WindowLocation("Tools");

	ImGui::SetWindowPos(ImVec2(toolsWindowLocation.xPosition, toolsWindowLocation.yPosition));
	ImGui::SetWindowSize(ImVec2(toolsWindowLocation.width, toolsWindowLocation.height));

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

	ImGui::End();

	bool activeToolWindowOpen = true;
	ImGui::Begin(activeTool->Name().c_str(), &activeToolWindowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	GUILocation activeToolWindowLocation = context->ApplicationGUIManager()->WindowLocation("ActiveTool");
	ImGui::SetWindowPos(ImVec2(activeToolWindowLocation.xPosition, activeToolWindowLocation.yPosition));
	ImGui::SetWindowSize(ImVec2(activeToolWindowLocation.width, activeToolWindowLocation.height));
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

	selectedToolButtonIndex = toolIndex;
}
