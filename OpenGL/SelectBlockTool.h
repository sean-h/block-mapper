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
	SelectBlockTool(ApplicationContext* context);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);

private:
	enum class SelectionModes
	{
		Single,
		Region,
		Border,
		Box,
	};

	void SelectSingle(EntitySelectionManager* selectionManager, Scene* scene, std::shared_ptr<EntityHandle> hitEntity);
	void SelectRegion(EntitySelectionManager* selectionManager, Scene* scene, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap);
	void SelectBorder(EntitySelectionManager* selectionManager, Scene* scene, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap);

	SelectionModes selectionMode;
	int selectionModeIndex;
	glm::vec2 selectBoxBegin;
	glm::vec2 selectBoxEnd;
};

std::vector<glm::ivec3> AdjacentPositions(glm::ivec3 position);