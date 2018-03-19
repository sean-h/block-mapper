#pragma once
#include "Entity.h"
#include <vector>
#include <memory>

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

private:
	void SelectedEntityListChanged();

	std::vector<std::shared_ptr<EntityHandle>> selectedEntities;
	std::string selectionCountText;
	std::string meshText;
	std::string colliderText;
	std::string uvText;
};