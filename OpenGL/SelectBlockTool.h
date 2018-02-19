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
	};

	void SelectSingle(EntitySelectionManager* selectionManager, std::shared_ptr<EntityHandle> hitEntity);
	void SelectRegion(EntitySelectionManager* selectionManager, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap);
	void SelectBorder(EntitySelectionManager* selectionManager, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap);

	SelectionModes selectionMode;
	int selectionModeIndex;
};

std::vector<glm::ivec3> AdjacentPositions(glm::ivec3 position);