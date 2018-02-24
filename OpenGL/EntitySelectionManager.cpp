#include "EntitySelectionManager.h"

void EntitySelectionManager::SelectEntity(std::shared_ptr<EntityHandle> entityHandle)
{
	int a = 1;
	int b = 2;

	if (entityHandle->EntityExists()
		&& entityHandle->TargetEntity()->HasProperty("Block")
		&& std::find(selectedEntities.begin(), selectedEntities.end(), entityHandle) == selectedEntities.end())
	{
		entityHandle->TargetEntity()->MaterialName("Selected");
		selectedEntities.push_back(entityHandle);
	}
}

void EntitySelectionManager::DeselectEntity(std::shared_ptr<EntityHandle> entityHandle)
{
	if (entityHandle->EntityExists())
	{
		entityHandle->TargetEntity()->MaterialName("Solid");
		selectedEntities.erase(std::remove(selectedEntities.begin(), selectedEntities.end(), entityHandle), selectedEntities.end());
	}
}

void EntitySelectionManager::DeselectAll()
{
	for (auto& e : selectedEntities)
	{
		if (e->EntityExists())
		{
			e->TargetEntity()->MaterialName("Solid");
		}
	}

	selectedEntities.clear();
}

std::vector<std::shared_ptr<EntityHandle>>& EntitySelectionManager::SelectedEntities()
{
	return selectedEntities;
}
