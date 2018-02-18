#pragma once
#include "Tool.h"

class ApplicationContext;

class SelectBlockTool : public Tool
{
public:
	SelectBlockTool(ApplicationContext* context);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);

private:

};