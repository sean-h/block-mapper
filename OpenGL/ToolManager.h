#pragma once
#include "Tool.h"
#include "imgui.h"
#include <memory>
#include <string>

class ApplicationContext;

class ToolManager
{
public:
	ToolManager(ApplicationContext* context);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);
	void Deserialize(ApplicationContext* context);

private:
	std::unique_ptr<Tool> activeTool;
	int selectedToolButtonIndex;
	ImTextureID iconTextureID;

	enum class ToolIndex
	{
		SelectSingle = 1,
		SelectRegion = 2,
		SelectBorder = 3,
		SelectBox = 4,
		SelectPlane = 5,
		Move = 10,
		DrawSingle = 20,
		DrawLine = 21,
		DrawRectangle = 22,
		Grid = 30,
		Extrude = 40,
	};

	bool ToggleButton(std::string text, int id, int& selectedButton);
	bool ToggleImageButton(int iconX, int iconY, std::string tooltip, int id, int& selectedButton);
	void SelectTool(ApplicationContext* context, ToolIndex toolIndex);
};