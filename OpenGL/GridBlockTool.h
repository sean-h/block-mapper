#pragma once
#include "Tool.h"
#include "Entity.h"
#include <memory>
#include <vector>
#include "glm\glm.hpp"

class GridBlockTool : public Tool
{
public:
	GridBlockTool(ApplicationContext* context);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);
	void RefreshHoverBlock(ApplicationContext* context);
	void DisableTool(ApplicationContext* context);

private:
	enum class BuildSteps
	{
		PlaceFirstBlock,
		PlaceLastBlock,
	};

	enum class Shapes
	{
		HollowCube,
		FilledCube
	};

	void PlaceHoverBlocks(ApplicationContext* context);
	void MoveBlock(ApplicationContext* context, glm::vec3 moveDirection);
	void Apply(ApplicationContext* context);
	void AdvanceBuildStep(ApplicationContext* context);

	glm::vec3 firstBlockPosition;
	glm::vec3 lastBlockPosition;
	std::vector<std::shared_ptr<EntityHandle>> hoverBlocks;
	BuildSteps buildStep;
	Shapes drawShape;
	std::string selectedBlockName;
	int selectedBlockColorIndex;
};