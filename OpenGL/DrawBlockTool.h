#pragma once
#include "Tool.h"
#include "Entity.h"

class DrawBlockTool : public Tool
{
public:
	DrawBlockTool(ApplicationContext* context);
	void Update(ApplicationContext* context);

private:
	Entity *hoverBlock;
};