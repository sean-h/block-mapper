#pragma once
#include "Tool.h"
#include "glm\glm.hpp"

class MoveBlockTool : public Tool
{
public:
	MoveBlockTool(ApplicationContext* context);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);

private:
	void MoveBlocks(ApplicationContext* context, glm::vec3 direction);
};