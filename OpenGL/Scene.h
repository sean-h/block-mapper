#pragma once
#include "glm\glm.hpp"
#include "Entity.h"
#include "Component.h"
#include "Camera.h"
#include <memory>
#include <vector>
#include <string>

class ApplicationContext;

class Scene
{
public:
	Scene();
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);
	std::vector<std::unique_ptr<Entity>>& Entities() { return entities; }
	int EntityCount() { return entities.size(); }

	std::shared_ptr<EntityHandle> CreateEntity();
	void DestroyEntity(std::shared_ptr<EntityHandle> entityHandle);
	Component* AddComponentToEntity(Entity* entity, std::unique_ptr<Component> component);
	Camera* ActiveCamera() { return camera; }
	void Export(ApplicationContext* context);
	std::string SceneName() const { return std::string(sceneName); }

private:
	std::vector<std::unique_ptr<Entity>> entities;
	int entityCounter;
	std::vector<std::unique_ptr<Component>> components;
	Camera* camera;
	char sceneName[64] = { 0 };
};