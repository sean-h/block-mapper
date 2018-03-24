#pragma once
#include "Tool.h"
#include "BlockManager.h"
#include "glm\glm.hpp"
#include <memory>
#include <unordered_map>

class ApplicationContext;
class EntitySelectionManager;
class EntityHandle;

class SelectBlockTool : public Tool
{
public:
	enum class SelectionModes
	{
		Single,
		Region,
		Border,
		Box,
		Plane,
	};

	SelectBlockTool(ApplicationContext* context);
	SelectBlockTool(ApplicationContext* context, SelectionModes selectionMode);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);

	enum class Planes
	{
		XY,
		XZ,
		YZ,
	};

private:
	void SelectSingle(EntitySelectionManager* selectionManager, Scene* scene, std::shared_ptr<EntityHandle> hitEntity);
	void SelectRegion(EntitySelectionManager* selectionManager, Scene* scene, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap);
	void SelectBorder(EntitySelectionManager* selectionManager, Scene* scene, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap);
	void SelectPlane(EntitySelectionManager* selectionManager, Scene* scene, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap);
	void SelectArea(EntitySelectionManager* selectionManager, Scene* scene, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap);

	SelectionModes selectionMode;
	int selectionModeIndex;
	glm::vec2 selectBoxBegin;
	glm::vec2 selectBoxEnd;
};

std::vector<glm::ivec3> AdjacentPositions(glm::ivec3 position);
std::vector<glm::ivec3> AdjacentPlanePositions(glm::ivec3 position, SelectBlockTool::Planes plane);