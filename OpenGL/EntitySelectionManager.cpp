#include "EntitySelectionManager.h"
#include "Scene.h"
#include "imgui.h"
#include "ApplicationContext.h"
#include <set>

void EntitySelectionManager::SelectEntity(Scene* scene, std::shared_ptr<EntityHandle> entityHandle)
{
	if (entityHandle->EntityExists()
		&& entityHandle->TargetEntity()->HasProperty("Block")
		&& std::find(selectedEntities.begin(), selectedEntities.end(), entityHandle) == selectedEntities.end())
	{
		entityHandle->TargetEntity()->MaterialName("Selected");
		scene->RefreshEntityRenderData(entityHandle);
		selectedEntities.push_back(entityHandle);
		SelectedEntityListChanged();
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
		SelectedEntityListChanged();
	}
}

void EntitySelectionManager::DeselectEntity(Scene* scene, std::shared_ptr<EntityHandle> entityHandle)
{
	if (entityHandle->EntityExists())
	{
		entityHandle->TargetEntity()->MaterialName("Solid");
		scene->RefreshEntityRenderData(entityHandle);
		selectedEntities.erase(std::remove(selectedEntities.begin(), selectedEntities.end(), entityHandle), selectedEntities.end());
		SelectedEntityListChanged();
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
	SelectedEntityListChanged();
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

	if (selectedEntities.size() > 0)
	{
		ImGui::Text(meshText.c_str());
		ImGui::Text(colliderText.c_str());
		ImGui::Text(uvText.c_str());

		if (ImGui::Button("Match Block Panel"))
		{
			BlockManager* blockManager = context->ApplicationBlockManager();
			Scene* scene = context->ApplicationScene();
			for (auto& e : selectedEntities)
			{
				if (e->EntityExists())
				{
					Entity* entity = e->TargetEntity();
					BlockPreset blockPreset = context->ApplicationBlockManager()->GetBlockPresetAtPosition(entity->ObjectTransform()->GridPosition());
					entity->MeshName(blockPreset.meshName);
					entity->ColliderMeshName(blockPreset.colliderName);
					entity->MeshColorIndex(blockPreset.colorIndex);
					scene->RefreshEntityRenderData(e);
					scene->RefreshEntityCollisionData(e);
				}
			}
		}
	}
}

std::shared_ptr<EntityHandle> EntitySelectionManager::LastSelectedEntity() const
{
	if (selectedEntities.size() == 0)
	{
		return nullptr;
	}

	return selectedEntities[selectedEntities.size() - 1];
}

void EntitySelectionManager::SelectedEntityListChanged()
{
	if (selectedEntities.size() > 0)
	{
		std::set<std::string> selectedMeshNames;
		std::set<std::string> selectedColliderNames;
		std::set<int> selectedUVs;

		for (auto& entityHandle : selectedEntities)
		{
			if (entityHandle->EntityExists())
			{
				Entity* entity = entityHandle->TargetEntity();
				selectedMeshNames.insert(entity->MeshName());
				selectedColliderNames.insert(entity->ColliderMeshName());
				selectedUVs.insert(entity->MeshColorIndex());
			}
		}

		meshText = "Mesh: ";
		if (selectedMeshNames.size() == 1)
		{
			meshText += *selectedMeshNames.begin();
		}
		else
		{
			meshText += "---";
		}

		colliderText = "Collider: ";
		if (selectedColliderNames.size() == 1)
		{
			colliderText += *selectedColliderNames.begin();
		}
		else
		{
			colliderText += "---";
		}

		uvText = "UV: ";
		if (selectedUVs.size() == 1)
		{
			uvText += std::to_string(*selectedUVs.begin());
		}
		else
		{
			uvText += "---";
		}
	}
}
