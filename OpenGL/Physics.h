#pragma once
#include "Entity.h"
#include <unordered_map>
#include <map>

class Scene;
class Debug;

struct RaycastHit
{
	bool hit;
	unsigned int physicsObjectID;
	std::shared_ptr<EntityHandle> entity;
	glm::vec3 normal;
	float u;
	float v;
	float t;
	glm::vec3 point;
};

struct Frustum
{
	glm::vec3 cameraPosition;
	float nearPlaneDistance;
	glm::vec3 nearPlanePosition;
	glm::vec3 nearPlaneNormal;
	float farPlaneDistance;
	glm::vec3 topLeft;
	glm::vec3 topRight;
	glm::vec3 bottomLeft;
	glm::vec3 bottomRight;
};

struct Plane
{
	Plane() { }
	Plane(glm::vec3 position, glm::vec3 normal) : position(position), normal(normal) { }
	glm::vec3 position;
	glm::vec3 normal;
	float d;
};

struct PhysicsObject
{
	unsigned int ID;
	glm::vec3 boundingBoxPosition;
	glm::vec3 boundingBoxScale;
	Mesh* collisionMesh;
};

class Physics
{
public:
	Physics();
	RaycastHit Raycast(Scene* scene, glm::vec3 origin, glm::vec3 direction, float distance);
	std::vector<std::shared_ptr<EntityHandle>> FrustumIntersect(Debug* debug, Scene* scene, Frustum frustum);

	unsigned int CreatePhysicsObject(glm::vec3 position, glm::vec3 scale, std::string colliderMeshName);
	void UpdatePhysicsObjectPosition(unsigned int physicsObjectID, glm::vec3 position);
	void DestroyPhysicsObject(unsigned int physicsObjectID);

private:
	void LoadColliders();
	std::unordered_map<std::string, Model*> colliders;
	RaycastHit RayTriIntersect(glm::vec3 origin, glm::vec3 direction, glm::vec3 p0, glm::vec3 p1, glm::vec3 p2);
	bool PlaneAABIntersectOrInside(glm::vec3 AABBMin, glm::vec3 AABBMax, Plane plane);

	unsigned int physicsObjectCounter;
	std::map<unsigned int, PhysicsObject> physicsObjects;
};
