#pragma once
#include "PlaceBlockTool.h"

class Scene;

class DrawBlockTool : public PlaceBlockTool
{
public:
	enum class DrawModes
	{
		Point,
		Line,
		Rectangle,
	};

	DrawBlockTool(ApplicationContext* context);
	DrawBlockTool(ApplicationContext* context, DrawModes drawMode);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);
	std::string Name() { return "Draw"; }

private:
	enum class BuildStates
	{
		PlaceFirstBlock,
		PlaceLastBlock,
		Completed,
		Canceled,
	};

	void RefreshGhostBlocks(ApplicationContext* context);
	void RotateGhostBlocks(ApplicationContext* context, glm::vec3 axis, float angleDegrees);
	bool GetNewGhostBlockPosition(ApplicationContext* context, glm::vec3& newBlockPosition);

	DrawModes drawMode;
	BuildStates buildState;
	glm::quat drawRotation;
	int selectedDrawModeIndex;
	std::string selectedBlockName;
	int selectedBlockColorIndex;

	glm::vec3 drawStartPosition;
	glm::vec3 drawEndPosition;
};