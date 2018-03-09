#pragma once
#include "PlaceBlockTool.h"
#include "glm\glm.hpp"
#include <vector>
#include <memory>

class EntityHandle;

class ExtrudeBlockTool : public PlaceBlockTool
{
public:
	ExtrudeBlockTool(ApplicationContext* context);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);
	std::string Name() { return "Extrude"; }

private:
	void MoveBlock(ApplicationContext* context, glm::vec3 direction);
	void RefreshGhostBlocks(ApplicationContext* context);

	std::vector<glm::vec3> extrudeFromPoints;
	std::vector<std::shared_ptr<EntityHandle>> hoverBlocks;
	glm::vec3 extrudeDirection;
	int extrudeDistance;
	std::string selectedBlockName;
	int selectedBlockColorIndex;
};