#pragma once
#include "glm\glm.hpp"
#include "Entity.h"
#include "Component.h"
#include "Camera.h"
#include <memory>
#include <vector>

class ApplicationContext;

class Scene
{
public:
	Scene();
	void Update(ApplicationContext* context);
	std::vector<std::unique_ptr<Entity>>& Entities() { return entities; }
	int EntityCount() { return entities.size(); }

	std::shared_ptr<EntityHandle> CreateEntity();
	void DestroyEntity(std::shared_ptr<EntityHandle> entityHandle);
	Component* AddComponentToEntity(Entity* entity, std::unique_ptr<Component> component);
	Camera* ActiveCamera() { return camera; }
	void Export(ApplicationContext* context);

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<Component>> components;
	Camera* camera;
};