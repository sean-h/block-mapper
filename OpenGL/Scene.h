#pragma once
#include "glm\glm.hpp"
#include "Entity.h"
#include <vector>

class Scene
{
public:
	Scene();
	std::vector<Entity> Entities() const { return entities; }

private:
	std::vector<Entity> entities;
};