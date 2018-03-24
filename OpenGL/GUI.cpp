#include "GUI.h"

bool GUI::ToggleButton(std::string text, int id, int & selectedButton, bool fillWidth)
{
	if (id == selectedButton)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.8f, 0.8f));
	}

	bool clicked = false;
	if (fillWidth)
	{
		clicked = ImGui::Button(text.c_str(), ImVec2(-1.0f, 25.0f));
	}
	else
	{
		clicked = ImGui::Button(text.c_str(), ImVec2(0.0f, 25.0f));
	}

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

bool GUI::SmallToggleButton(std::string text, int id, int & selectedButton)
{
	if (id == selectedButton)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.6f, 0.6f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.7f, 0.7f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(1.0f / 7.0f, 0.8f, 0.8f));
	}

	bool clicked = ImGui::SmallButton(text.c_str());

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
