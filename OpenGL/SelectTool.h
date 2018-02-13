#pragma once
#include "Tool.h"

class ApplicationContext;

class SelectTool : public Tool
{
public:
	SelectTool(ApplicationContext* context);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);

private:

};