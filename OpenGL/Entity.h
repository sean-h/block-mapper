#pragma once
#include <string>
#include "Transform.h"
#include "Model.h"

class Entity
{
public:
	Transform* ObjectTransform() { return &transform; }

	std::string MeshName() const { return meshName; }
	void MeshName(std::string meshName) { this->meshName = meshName; }

	std::string ColliderMeshName() const { return colliderMeshName; }
	void ColliderMeshName(std::string colliderMeshName) { this->colliderMeshName = colliderMeshName; }

private:
	Transform transform;
	std::string meshName;
	std::string colliderMeshName;
};