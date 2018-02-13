#pragma once
#include "Tool.h"
#include <memory>
#include <string>

class ApplicationContext;

class ToolManager
{
public:
	ToolManager(ApplicationContext* context);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);

private:
	std::unique_ptr<Tool> activeTool;
	int selectedToolButtonIndex;

	bool ToggleButton(std::string text, int id, int& selectedButton);
};