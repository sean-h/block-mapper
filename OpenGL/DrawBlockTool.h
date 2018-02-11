#pragma once
#include "Tool.h"
#include "Entity.h"

class Scene;

class DrawBlockTool : public Tool
{
public:
	DrawBlockTool(ApplicationContext* context);
	void Update(ApplicationContext* context);
	void PlaceBlock(Scene* scene);
	void RefreshHoverBlock(ApplicationContext* context);

private:
	Entity* hoverBlock;
	Entity* gridPlane;
	Entity* gridPlaneBottom;
};