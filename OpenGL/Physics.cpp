#include "Physics.h"
#include "Scene.h"
#include <iostream>

Physics::Physics()
{
	LoadColliders();
}

RaycastHit Physics::Raycast(Scene * scene, glm::vec3 origin, glm::vec3 direction, float distance)
{
	std::vector<std::pair<Entity*, float>> hitEntities;

	// See Real-Time Rendering Third Edition 16.7.2
	for (auto &e : scene->Entities())
	{
		if (e->ColliderMeshName().length() == 0)
		{
			continue;
		}

		glm::vec3 entityPosition = e->ObjectTransform()->Position();
		glm::vec3 endPoint0 = origin - entityPosition;
		glm::vec3 endPoint1 = origin - entityPosition + (direction * distance);
		glm::vec3 c = (endPoint0 + endPoint1) / 2.0f;
		glm::vec3 w = endPoint1 - c;
		float vx = abs(w.x);
		float vy = abs(w.y);
		float vz = abs(w.z);
		glm::vec3 entityScale = e->ObjectTransform()->Scale();
		float hx = entityScale.x / 2.0f;
		float hy = entityScale.y / 2.0f;
		float hz = entityScale.z / 2.0f;

		if (abs(c.x) > vx + hx || abs(c.y) > vy + hy || abs(c.z) > vz + hz)
		{
			continue;
		}

		if (abs(c.y * w.z - c.z * w.y) > (hy * vz) + (hz * vy))
		{
			continue;
		}

		if (abs(c.x * w.z - c.z * w.x) > (hx * vz) + (hz * vx))
		{
			continue;
		}

		if (abs(c.x * w.y - c.y * w.x) > (hx * vy) + (hy * vx))
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

		Model* model = this->colliders[closestEntity.first->ColliderMeshName()];
		glm::vec3 entityPosition = closestEntity.first->ObjectTransform()->Position();
		glm::mat4 modelMatrix = closestEntity.first->ObjectTransform()->Model();
		for (auto& mesh : model->Meshes())
		{
			auto triangles = mesh.Triangles();
			for (auto& triangle : triangles)
			{
				RaycastHit hit = RayTriIntersect(origin,
					                             direction,
					                             modelMatrix * glm::vec4(triangle.p0, 1.0f),
					                             modelMatrix * glm::vec4(triangle.p1, 1.0f),
					                             modelMatrix * glm::vec4(triangle.p2, 1.0f));
				if (hit.hit)
				{
					hit.entity = closestEntity.first->Handle();
					return hit;
				}
			}
		}
	}

	RaycastHit hit;
	hit.hit = false;
	return hit;
}

void Physics::LoadColliders()
{
	this->colliders["Cube"] = new Model("Cube.fbx");
	this->colliders["Plane"] = new Model("Plane.fbx");
	this->colliders["PlaneBottom"] = new Model("PlaneBottom.fbx");
}

RaycastHit Physics::RayTriIntersect(glm::vec3 origin, glm::vec3 direction, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
	RaycastHit hit;
	glm::vec3 e1 = p1 - p0;
	glm::vec3 e2 = p2 - p0;
	glm::vec3 normal = glm::normalize(glm::cross(e1, e2));

	if (glm::dot(normal, direction) > 0.0f)
	{
		hit.hit = false;
		return hit;
	}

	glm::vec3 q = glm::cross(direction, e2);
	float a = glm::dot(e1, q);

	if (a > -FLT_EPSILON && a < FLT_EPSILON)
	{
		hit.hit = false;
		return hit;
	}

	float f = 1.0f / a;
	glm::vec3 s = origin - p0;
	float u = f * (glm::dot(s, q));
	if (u < 0.0f)
	{
		hit.hit = false;
		return hit;
	}
	glm::vec3 r = glm::cross(s, e1);
	float v = f * (glm::dot(direction, r));
	if (v < 0.0f || u + v > 1.0)
	{
		hit.hit = false;
		return hit;
	}
	float t = f * (glm::dot(e2, r));

	hit.hit = true;
	hit.normal = normal;
	hit.u = u;
	hit.v = v;
	hit.t = t;
	hit.point = (1 - u - v) * p0 + u * p1 + v * p2;
	return hit;
}
