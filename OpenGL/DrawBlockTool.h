#pragma once
#include "Tool.h"
#include "Entity.h"

class Scene;

class DrawBlockTool : public Tool
{
public:
	DrawBlockTool(ApplicationContext* context);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);
	std::string Name() { return "Draw"; }
	void PlaceBlock(Scene* scene);
	void RefreshHoverBlock(ApplicationContext* context);
	void DisableTool(ApplicationContext* context);

private:
	std::shared_ptr<EntityHandle> hoverBlock;
};