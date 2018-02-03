#include "Scene.h"

Scene::Scene()
{
	std::vector<glm::vec3> cubePositions;
	cubePositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	cubePositions.push_back(glm::vec3(2.0f, 5.0f, -15.0f));
	cubePositions.push_back(glm::vec3(-1.0f, -2.0f, -2.0f));
	cubePositions.push_back(glm::vec3(-4.0f, -2.0f, -12.0f));
	cubePositions.push_back(glm::vec3(2.0f, 0.0f, -3.0f));
	cubePositions.push_back(glm::vec3(2.0f, 0.0f, -4.0f));
	cubePositions.push_back(glm::vec3(3.0f, 0.0f, -3.0f));
	cubePositions.push_back(glm::vec3(2.0f, -0.0f, -3.0f));
	cubePositions.push_back(glm::vec3(-2.0f, 3.0f, -7.0f));
	cubePositions.push_back(glm::vec3(1.0f, -2.0f, -2.0f));
	cubePositions.push_back(glm::vec3(1.0f, 2.0f, -2.0f));
	cubePositions.push_back(glm::vec3(1.0f, 0.2f, -1.0f));
	cubePositions.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));

	for (auto p : cubePositions)
	{
		Entity entity;
		entity.ObjectTransform().Position(p);
		entity.MeshName("Cube");
		this->entities.push_back(entity);
	}
}
