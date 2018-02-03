#pragma once
#include "glm\glm.hpp"
#include "Entity.h"
#include <memory>
#include <vector>

class Scene
{
public:
	Scene();
	std::vector<std::unique_ptr<Entity>>& Entities() { return entities; }
	Entity* CreateEntity();

private:
	std::vector<std::unique_ptr<Entity>> entities;
};