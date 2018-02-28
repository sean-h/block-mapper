#pragma once
#include "Tool.h"
#include "Entity.h"
#include "glm\gtc\matrix_transform.hpp"

class Scene;

class DrawBlockTool : public Tool
{
public:
	DrawBlockTool(ApplicationContext* context);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);
	std::string Name() { return "Draw"; }
	void PlaceBlocks(Scene* scene);
	void RefreshHoverBlocks(ApplicationContext* context);
	void DisableTool(ApplicationContext* context);

private:
	enum class DrawModes
	{
		Point,
		Line,
		Rectangle,
	};

	std::vector<std::shared_ptr<EntityHandle>> hoverBlocks;
	DrawModes drawMode;
	glm::quat drawRotation;
	int selectedDrawModeIndex;

	glm::vec3 drawStartPosition;
	glm::vec3 drawEndPosition;
};