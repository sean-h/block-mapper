#include "SelectTool.h"
#include "imgui.h"

SelectTool::SelectTool(ApplicationContext * context)
{
}

void SelectTool::Update(ApplicationContext * context)
{
}

void SelectTool::DrawGUI(ApplicationContext * context)
{
	bool selectToolWindowOpen = true;
	ImGui::Begin("Select", &selectToolWindowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	ImGui::SetWindowPos(ImVec2(10.0f, 250.0f));
	ImGui::SetWindowSize(ImVec2(150.0f, 200.0f));

	ImGui::Text("Select");

	ImGui::End();
}
