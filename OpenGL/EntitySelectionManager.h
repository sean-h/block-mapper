#pragma once
#include "Entity.h"
#include <vector>
#include <memory>
#include <set>

class Scene;
class ApplicationContext;

class EntitySelectionManager
{
public:
	void SelectEntity(Scene* scene, std::shared_ptr<EntityHandle> entityHandle);
	void ToggleSelectEntity(Scene* scene, std::shared_ptr<EntityHandle> entityHandle);
	void DeselectEntity(Scene* scene, std::shared_ptr<EntityHandle> entityHandle);
	void DeselectAll(Scene* scene);
	std::vector<std::shared_ptr<EntityHandle>>& SelectedEntities();
	void DrawGUI(ApplicationContext* context);
	std::shared_ptr<EntityHandle> LastSelectedEntity() const;
	void OnSceneLoaded(Scene* scene);

private:
	void SelectedEntityListChanged();

	std::vector<std::shared_ptr<EntityHandle>> selectedEntities;
	std::string selectionCountText;
	std::string meshText;
	std::string colliderText;
	std::string uvText;
	const int propertyBufferSize = 128;
	std::unordered_map<EntityProperty, std::string> entityProperties;
	std::unordered_map<EntityProperty, std::unique_ptr<char[]>> entityPropertyBuffers;
	std::unordered_map<EntityProperty, bool> entityPropertyValuesEqual;
};