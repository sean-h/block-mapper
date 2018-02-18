#pragma once
#include "Entity.h"
#include <vector>
#include <memory>

class EntitySelectionManager
{
public:
	void SelectEntity(std::shared_ptr<EntityHandle> entityHandle);
	void DeselectEntity(std::shared_ptr<EntityHandle> entityHandle);
	void DeselectAll();
	std::vector<std::shared_ptr<EntityHandle>>& SelectedEntities();

private:
	std::vector<std::shared_ptr<EntityHandle>> selectedEntities;
};