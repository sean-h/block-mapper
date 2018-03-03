#pragma once
#include "Entity.h"
#include <vector>
#include <memory>

class Scene;

class EntitySelectionManager
{
public:
	void SelectEntity(Scene* scene, std::shared_ptr<EntityHandle> entityHandle);
	void DeselectEntity(Scene* scene, std::shared_ptr<EntityHandle> entityHandle);
	void DeselectAll(Scene* scene);
	std::vector<std::shared_ptr<EntityHandle>>& SelectedEntities();

private:
	std::vector<std::shared_ptr<EntityHandle>> selectedEntities;
};