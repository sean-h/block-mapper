#include "Physics.h"
#include "Scene.h"
#include "Debug.h"
#include "FileManager.h"
#include <iostream>

Physics::Physics(FileManager* fileManager)
{
	LoadColliders(fileManager);

	physicsObjectCounter = 0;
}

RaycastHit Physics::Raycast(Scene * scene, glm::vec3 origin, glm::vec3 direction, float distance)
{
	std::vector<PhysicsObject> hitObjects;

	// See Real-Time Rendering Third Edition 16.7.2
	for (auto &physicsObjectPair : physicsObjects)
	{
		PhysicsObject physicsObject = physicsObjectPair.second;

		glm::vec3 entityPosition = physicsObject.boundingBoxPosition;
		glm::vec3 endPoint0 = origin - entityPosition;
		glm::vec3 endPoint1 = origin - entityPosition + (direction * distance);
		glm::vec3 c = (endPoint0 + endPoint1) / 2.0f;
		glm::vec3 w = endPoint1 - c;
		float vx = abs(w.x);
		float vy = abs(w.y);
		float vz = abs(w.z);
		glm::vec3 entityScale = physicsObject.boundingBoxScale;
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
		hitObjects.push_back(physicsObjectPair.second);
	}

	RaycastHit hit;
	hit.hit = false;

	for (auto& hitObject : hitObjects)
	{
		glm::mat4 modelMatrix;
		modelMatrix = glm::translate(modelMatrix, hitObject.boundingBoxPosition);
		modelMatrix = glm::scale(modelMatrix, hitObject.boundingBoxScale);
		Mesh* mesh = hitObject.collisionMesh;
		auto triangles = mesh->Triangles();
		for (auto& triangle : triangles)
		{
			RaycastHit triHit = RayTriIntersect(origin,
					                            direction,
					                            modelMatrix * glm::vec4(triangle.p0, 1.0f),
					                            modelMatrix * glm::vec4(triangle.p1, 1.0f),
					                            modelMatrix * glm::vec4(triangle.p2, 1.0f));
			if (triHit.hit && (!hit.hit || triHit.distance < hit.distance))
			{
				hit = triHit;
				hit.physicsObjectID = hitObject.ID;
			}
		}
	}

	return hit;
}

std::vector<std::shared_ptr<EntityHandle>> Physics::FrustumIntersect(Debug* debug, Scene * scene, Frustum frustum)
{
	std::vector<std::shared_ptr<EntityHandle>> intersected;

	for (auto& entity : scene->Entities())
	{
		if (entity->ColliderMeshName().length() == 0 || entity->HasProperty("Hidden"))
		{
			continue;
		}

		glm::vec3 entityPosition = entity->ObjectTransform()->Position();

		glm::vec3 topLeftPos = frustum.cameraPosition + (frustum.topLeft * frustum.nearPlaneDistance);
		glm::vec3 topRightPos = frustum.cameraPosition + (frustum.topRight * frustum.nearPlaneDistance);
		glm::vec3 bottomLeftPos = frustum.cameraPosition + (frustum.bottomLeft * frustum.nearPlaneDistance);
		glm::vec3 bottomRightPos = frustum.cameraPosition + (frustum.bottomRight * frustum.nearPlaneDistance);

		Plane frontPlane(frustum.nearPlanePosition, frustum.nearPlaneNormal);
		Plane backPlane(frustum.nearPlanePosition + (-frustum.nearPlaneNormal * frustum.farPlaneDistance), -frustum.nearPlaneNormal);
		Plane topPlane(topLeftPos, glm::normalize(glm::cross(topRightPos - topLeftPos, frustum.topLeft)));
		Plane bottomPlane(bottomLeftPos, glm::normalize(glm::cross(frustum.bottomLeft, bottomRightPos - bottomLeftPos)));
		Plane leftPlane(topLeftPos, glm::normalize(glm::cross(frustum.topLeft, bottomLeftPos - topLeftPos)));
		Plane rightPlane(topRightPos, glm::normalize(glm::cross(bottomRightPos - topRightPos, frustum.topRight)));

		Plane frustumPlanes[6] = { frontPlane, backPlane, topPlane, bottomPlane, leftPlane, rightPlane };

		glm::vec3 AABBMax = entityPosition - glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 AABBMin = entityPosition + glm::vec3(0.5f, 0.5f, 0.5f);

		int planesWithin = 0;
		for (int i = 0; i < 6; i++)
		{
			if (this->PlaneAABIntersectOrInside(AABBMax, AABBMin, frustumPlanes[i]))
			{
				planesWithin++;
			}
		}

		if (planesWithin == 6)
		{
			intersected.push_back(entity->Handle());
		}
	}

	return intersected;
}

unsigned int Physics::CreatePhysicsObject(glm::vec3 position, glm::vec3 scale, std::string colliderMeshName)
{
	PhysicsObject physicsObject;
	physicsObject.boundingBoxPosition = position;
	physicsObject.boundingBoxScale = scale;
	physicsObject.collisionMesh = this->colliders[colliderMeshName]->GetMesh(0);
	physicsObject.ID = ++physicsObjectCounter;
	this->physicsObjects[physicsObject.ID] = physicsObject;
	return physicsObject.ID;
}

void Physics::UpdatePhysicsObjectPosition(unsigned int physicsObjectID, glm::vec3 position)
{
	if (physicsObjectID == 0)
	{
		return;
	}

	physicsObjects[physicsObjectID].boundingBoxPosition = position;
}

void Physics::DestroyPhysicsObject(unsigned int physicsObjectID)
{
	if (physicsObjectID == 0)
	{
		return;
	}

	physicsObjects.erase(physicsObjectID);
}

void Physics::LoadColliders(FileManager* fileManager)
{
	this->colliders["Cube"] = new Model("Cube.fbx");
	this->colliders["Plane"] = new Model("Plane.fbx");
	this->colliders["PlaneBottom"] = new Model("PlaneBottom.fbx");

	for (auto &collider : fileManager->BlockPaths())
	{
		this->colliders[collider.first] = new Model(collider.second);

		if (this->colliders[collider.first]->ActiveMesh() == nullptr)
		{
			std::cout << "Could not load collider: " << collider.first << std::endl;
			colliders.erase(collider.first);
		}
	}
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
	hit.distance = glm::distance(origin, hit.point);
	return hit;
}

bool Physics::PlaneAABIntersectOrInside(glm::vec3 AABBMin, glm::vec3 AABBMax, Plane plane)
{
	glm::vec3 c = (AABBMax + AABBMin) / 2.0f;
	glm::vec3 h = (AABBMax - AABBMin) / 2.0f;
	float e = h.x * glm::abs(plane.normal.x) + h.y * glm::abs(plane.normal.y) + h.z * glm::abs(plane.normal.z);
	float d = -glm::dot(plane.position, plane.normal);
	float s = glm::dot(c, plane.normal) + d;

	if (s - e > 0)
	{
		return false;
	}

	return true;
}
