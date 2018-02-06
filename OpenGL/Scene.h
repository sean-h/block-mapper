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
	Entity* CreateEntity();
	Component* AddComponentToEntity(Entity* entity, std::unique_ptr<Component> component);
	Camera* ActiveCamera() { return this->camera.get(); }

private:
	std::vector<std::unique_ptr<Entity>> entities;
	std::vector<std::unique_ptr<Component>> components;
	std::unique_ptr<Camera> camera;
};