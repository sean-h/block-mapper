#pragma once
#include "Entity.h"
#include "Scene.h"
#include <unordered_map>

class Physics
{
public:
	Physics();
	Entity * Raycast(Scene* scene, glm::vec3 origin, glm::vec3 direction, float distance);

private:
	void LoadColliders();
	std::unordered_map<std::string, Model*> colliders;
	bool RayTriIntersect(glm::vec3 origin, glm::vec3 direction, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
};