#pragma once
#include "Entity.h"
#include <unordered_map>

class Scene;

struct RaycastHit
{
	bool hit;
	Entity* entity;
	glm::vec3 normal;
	float u;
	float v;
	float t;
	glm::vec3 point;
};

class Physics
{
public:
	Physics();
	RaycastHit Raycast(Scene* scene, glm::vec3 origin, glm::vec3 direction, float distance);

private:
	void LoadColliders();
	std::unordered_map<std::string, Model*> colliders;
	RaycastHit RayTriIntersect(glm::vec3 origin, glm::vec3 direction, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
};
