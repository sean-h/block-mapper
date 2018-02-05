#pragma once
#include "Tool.h"

class DrawBlockTool : public Tool
{
public:
	DrawBlockTool(Scene* scene);
	void Update(Scene* scene, Input* input, Physics* physics);

private:
	Entity *hoverBlock;
};