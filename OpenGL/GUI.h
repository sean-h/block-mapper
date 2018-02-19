#pragma once
#include "imgui.h"
#include <string>

namespace GUI
{
	bool ToggleButton(std::string text, int id, int & selectedButton)
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
}