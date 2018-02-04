#include "Physics.h"
#include <iostream>

Physics::Physics()
{
	LoadColliders();
}

Entity * Physics::Raycast(Scene * scene, glm::vec3 origin, glm::vec3 direction, float distance)
{
	std::vector<std::pair<Entity*, float>> hitEntities;

	// See Real-Time Rendering Third Edition 16.7.2
	for (auto &e : scene->Entities())
	{
		glm::vec3 entityPosition = e->ObjectTransform().Position();
		glm::vec3 endPoint0 = origin - entityPosition;
		glm::vec3 endPoint1 = origin - entityPosition + (direction * distance);
		glm::vec3 c = (endPoint0 + endPoint1) / 2.0f;
		glm::vec3 w = endPoint1 - c;
		float vx = abs(w.x);
		float vy = abs(w.y);
		float vz = abs(w.z);

		if (abs(c.x) > vx + 0.5f || abs(c.y) > vy + 0.5f || abs(c.z) > vz + 0.5f)
		{
			continue;
		}

		if (abs(c.y * w.z - c.z * w.y) > (0.5f * vz) + (0.5f * vy))
		{
			continue;
		}

		if (abs(c.x * w.z - c.z * w.x) > (0.5f * vz) + (0.5f * vx))
		{
			continue;
		}

		if (abs(c.x * w.y - c.y * w.x) > (0.5f * vy) + (0.5f * vx))
		{
			continue;
		}
		
		float entityDistance = glm::distance(origin, entityPosition);
		hitEntities.push_back(std::make_pair<Entity*, float>(e.get(), (float)entityDistance));
	}

	if (hitEntities.size() > 0)
	{
		std::pair<Entity*, float> closestEntity = hitEntities.front();

		for (int i = 1; i < hitEntities.size(); i++)
		{
			if (hitEntities[i].second < closestEntity.second)
			{
				closestEntity = hitEntities[i];
			}
		}

		Model* model = this->colliders["Cube"];
		glm::vec3 entityPosition = closestEntity.first->ObjectTransform().Position();
		for (auto& mesh : model->Meshes())
		{
			auto triangles = mesh.Triangles();
			for (auto& triangle : triangles)
			{
				if (RayTriIntersect(origin, direction, triangle.p0 + entityPosition, triangle.p1 + entityPosition, triangle.p2 + entityPosition))
				{
					glm::vec3 e1 = triangle.p1 - triangle.p0;
					glm::vec3 e2 = triangle.p2 - triangle.p0;
					glm::vec3 normal = glm::cross(e2, e1);
					std::cout << "Normal: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
				}
			}
		}

		return closestEntity.first;
	}

	return nullptr;
}

void Physics::LoadColliders()
{
	this->colliders["Cube"] = new Model("Cube.fbx");
}

bool Physics::RayTriIntersect(glm::vec3 origin, glm::vec3 direction, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
	glm::vec3 e1 = p1 - p0;
	glm::vec3 e2 = p2 - p0;
	glm::vec3 normal = glm::cross(e2, e1);

	if (glm::dot(normal, direction) > 0.0f)
	{
		return false;
	}

	glm::vec3 q = glm::cross(direction, e2);
	float a = glm::dot(e1, q);

	if (a > -FLT_EPSILON && a < FLT_EPSILON)
	{
		return false;
	}

	float f = 1.0f / a;
	glm::vec3 s = origin - p0;
	float u = f * (glm::dot(s, q));
	if (u < 0.0f)
	{
		return false;
	}
	glm::vec3 r = glm::cross(s, e1);
	float v = f * (glm::dot(direction, r));
	if (v < 0.0f || u + v > 1.0)
	{
		return false;
	}
	float t = f * (glm::dot(e2, r));

	return true;
}
