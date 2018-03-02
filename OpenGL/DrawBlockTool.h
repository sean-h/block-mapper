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

	enum class BuildStates
	{
		PlaceFirstBlock,
		PlaceLastBlock,
		Completed,
		Canceled,
	};

	void ClearHoverBlocks(Scene* scene);
	void RotateHoverBlocks(ApplicationContext* context, glm::vec3 axis, float angleDegrees);
	bool GetHoverBlockPosition(ApplicationContext* context, glm::vec3& newBlockPosition);

	std::vector<std::shared_ptr<EntityHandle>> hoverBlocks;
	DrawModes drawMode;
	BuildStates buildState;
	glm::quat drawRotation;
	int selectedDrawModeIndex;
	std::string selectedBlockName;
	int selectedBlockColorIndex;

	glm::vec3 drawStartPosition;
	glm::vec3 drawEndPosition;
};