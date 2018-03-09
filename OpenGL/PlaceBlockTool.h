#pragma once
#include "Tool.h"
#include "glm\glm.hpp"
#include "Entity.h"
#include <vector>
#include <stack>
#include <memory>

class ApplicationContext;
class Scene;

class PlaceBlockTool : public Tool
{
public:
	void DisableTool(ApplicationContext* context);

protected:
	virtual void Apply(ApplicationContext* context);
	void PlaceGhostBlock(ApplicationContext* context, glm::vec3 position, glm::quat rotation);
	virtual void RefreshGhostBlocks(ApplicationContext* context) {}
	void ClearGhostBlocks(Scene* scene);

	std::vector<std::shared_ptr<EntityHandle>> ghostBlocks;
	std::stack<std::shared_ptr<EntityHandle>> ghostBlockPool;
};