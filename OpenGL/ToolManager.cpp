#include "ToolManager.h"
#include "DrawBlockTool.h"
#include "SelectBlockTool.h"
#include "GridBlockTool.h"
#include "MoveBlockTool.h"
#include "ExtrudeBlockTool.h"
#include "ApplicationContext.h"

ToolManager::ToolManager(ApplicationContext* context)
{
	this->selectedToolButtonIndex = (int)ToolIndex::DrawRectangle;
	SelectTool(context, ToolIndex::DrawRectangle);
	iconTextureID = (ImTextureID)context->ApplicationRenderer()->IconTextureID();
}

void ToolManager::Update(ApplicationContext * context)
{
	if (activeTool)
	{
		activeTool->Update(context);
	}
}

void ToolManager::DrawGUI(ApplicationContext * context)
{
	ImGui::Text("Tools");

	// Select Tools

	// Select Single
	if (this->ToggleImageButton(0, 0, "Select Single", (int)ToolIndex::SelectSingle, selectedToolButtonIndex))
		this->SelectTool(context, ToolIndex::SelectSingle);
	ImGui::SameLine();
	if (this->ToggleImageButton(1, 0, "Select Region", (int)ToolIndex::SelectRegion, selectedToolButtonIndex))
		this->SelectTool(context, ToolIndex::SelectRegion);
	ImGui::SameLine();
	if (this->ToggleImageButton(2, 0, "Select Border", (int)ToolIndex::SelectBorder, selectedToolButtonIndex))
		this->SelectTool(context, ToolIndex::SelectBorder);

	if (this->ToggleImageButton(3, 0, "Select Box", (int)ToolIndex::SelectBox, selectedToolButtonIndex))
		this->SelectTool(context, ToolIndex::SelectBox);
	ImGui::SameLine();
	if (this->ToggleImageButton(4, 0, "Select Plane", (int)ToolIndex::SelectPlane, selectedToolButtonIndex))
		this->SelectTool(context, ToolIndex::SelectPlane);

	if (this->ToggleImageButton(0, 1, "Move", (int)ToolIndex::Move, selectedToolButtonIndex))
		this->SelectTool(context, ToolIndex::Move);

	if (this->ToggleImageButton(0, 2, "Draw Single", (int)ToolIndex::DrawSingle, selectedToolButtonIndex))
		this->SelectTool(context, ToolIndex::DrawSingle);
	ImGui::SameLine();
	if (this->ToggleImageButton(1, 2, "Draw Line", (int)ToolIndex::DrawLine, selectedToolButtonIndex))
		this->SelectTool(context, ToolIndex::DrawLine);
	ImGui::SameLine();
	if (this->ToggleImageButton(2, 2, "Draw Rectangle", (int)ToolIndex::DrawRectangle, selectedToolButtonIndex))
		this->SelectTool(context, ToolIndex::DrawRectangle);

	if (this->ToggleImageButton(0, 3, "Draw Grid", (int)ToolIndex::Grid, selectedToolButtonIndex))
		this->SelectTool(context, ToolIndex::Grid);

	if (this->ToggleImageButton(0, 4, "Extrude", (int)ToolIndex::Extrude, selectedToolButtonIndex))
		this->SelectTool(context, ToolIndex::Extrude);
}

void ToolManager::Deserialize(ApplicationContext* context)
{
	activeTool = std::make_unique<DrawBlockTool>(DrawBlockTool(context));
	this->selectedToolButtonIndex = 2;
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

bool ToolManager::ToggleImageButton(int iconX, int iconY, std::string tooltip, int id, int & selectedButton)
{
	if (id == selectedButton)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.8f, 0.8f));
	}

	ImVec2 uvStart(1.0f / 16.0f * (float)iconX, 1.0f / 16.0f * (float)iconY);
	ImVec2 uvEnd = ImVec2(uvStart.x + 1.0f / 16.0f, uvStart.y + 1.0f / 16.0f);

	std::string idString = "##ToggleImageButton" + std::to_string(id);
	ImGui::PushID(idString.c_str());
	bool clicked = ImGui::ImageButton(iconTextureID, ImVec2(20.0f, 20.0f), uvStart, uvEnd);
	ImGui::PopID();
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip(tooltip.c_str());

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

void ToolManager::SelectTool(ApplicationContext* context, ToolIndex toolIndex)
{
	if (activeTool)
	{
		activeTool->DisableTool(context);
		activeTool.reset();
	}

	switch (toolIndex)
	{
	case ToolIndex::SelectSingle:
		activeTool = std::make_unique<SelectBlockTool>(SelectBlockTool(context, SelectBlockTool::SelectionModes::Single));
		break;
	case ToolIndex::SelectBorder:
		activeTool = std::make_unique<SelectBlockTool>(SelectBlockTool(context, SelectBlockTool::SelectionModes::Border));
		break;
	case ToolIndex::SelectBox:
		activeTool = std::make_unique<SelectBlockTool>(SelectBlockTool(context, SelectBlockTool::SelectionModes::Box));
		break;
	case ToolIndex::SelectRegion:
		activeTool = std::make_unique<SelectBlockTool>(SelectBlockTool(context, SelectBlockTool::SelectionModes::Region));
		break;
	case ToolIndex::SelectPlane:
		activeTool = std::make_unique<SelectBlockTool>(SelectBlockTool(context, SelectBlockTool::SelectionModes::Plane));
		break;
	case ToolIndex::Move:
		activeTool = std::make_unique<MoveBlockTool>(MoveBlockTool(context));
		break;
	case ToolIndex::DrawSingle:
		activeTool = std::make_unique<DrawBlockTool>(DrawBlockTool(context, DrawBlockTool::DrawModes::Point));
		break;
	case ToolIndex::DrawLine:
		activeTool = std::make_unique<DrawBlockTool>(DrawBlockTool(context, DrawBlockTool::DrawModes::Line));
		break;
	case ToolIndex::DrawRectangle:
		activeTool = std::make_unique<DrawBlockTool>(DrawBlockTool(context, DrawBlockTool::DrawModes::Rectangle));
		break;
	case ToolIndex::Grid:
		activeTool = std::make_unique<GridBlockTool>(GridBlockTool(context));
		break;
	case ToolIndex::Extrude:
		activeTool = std::make_unique<ExtrudeBlockTool>(ExtrudeBlockTool(context));
		break;
	default:
		break;
	}

	selectedToolButtonIndex = (int)toolIndex;
}
