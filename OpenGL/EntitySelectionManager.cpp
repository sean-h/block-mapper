#include "EntitySelectionManager.h"
#include "Scene.h"
#include "imgui.h"

void EntitySelectionManager::SelectEntity(Scene* scene, std::shared_ptr<EntityHandle> entityHandle)
{
	if (entityHandle->EntityExists()
		&& entityHandle->TargetEntity()->HasProperty("Block")
		&& std::find(selectedEntities.begin(), selectedEntities.end(), entityHandle) == selectedEntities.end())
	{
		entityHandle->TargetEntity()->MaterialName("Selected");
		scene->RefreshEntityRenderData(entityHandle);
		selectedEntities.push_back(entityHandle);
	}
}

void EntitySelectionManager::ToggleSelectEntity(Scene * scene, std::shared_ptr<EntityHandle> entityHandle)
{
	if (entityHandle->EntityExists() && entityHandle->TargetEntity()->HasProperty("Block"))
	{
		auto findEntity = std::find(selectedEntities.begin(), selectedEntities.end(), entityHandle);
		if (findEntity == selectedEntities.end())
		{
			entityHandle->TargetEntity()->MaterialName("Selected");
			selectedEntities.push_back(entityHandle);
		}
		else
		{
			entityHandle->TargetEntity()->MaterialName("Solid");
			selectedEntities.erase(findEntity);
		}
		
		scene->RefreshEntityRenderData(entityHandle);
	}
}

void EntitySelectionManager::DeselectEntity(Scene* scene, std::shared_ptr<EntityHandle> entityHandle)
{
	if (entityHandle->EntityExists())
	{
		entityHandle->TargetEntity()->MaterialName("Solid");
		scene->RefreshEntityRenderData(entityHandle);
		selectedEntities.erase(std::remove(selectedEntities.begin(), selectedEntities.end(), entityHandle), selectedEntities.end());
	}
}

void EntitySelectionManager::DeselectAll(Scene* scene)
{
	for (auto& e : selectedEntities)
	{
		if (e->EntityExists())
		{
			e->TargetEntity()->MaterialName("Solid");
			scene->RefreshEntityRenderData(e);
		}
	}

	selectedEntities.clear();
}

std::vector<std::shared_ptr<EntityHandle>>& EntitySelectionManager::SelectedEntities()
{
	return selectedEntities;
}

void EntitySelectionManager::DrawGUI(ApplicationContext * context)
{
	ImGui::Text("Entity Properties");
	std::string selectionCount = "Count: " + std::to_string(selectedEntities.size());
	ImGui::Text(selectionCount.c_str());
}
