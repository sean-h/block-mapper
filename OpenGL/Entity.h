#pragma once
#include <string>
#include <memory>
#include "Transform.h"
#include "Model.h"

class Entity;

class EntityHandle
{
	friend class Entity;
public:
	EntityHandle(Entity* entity) { this->entity = entity; }
	Entity* TargetEntity() { return entity; }
	bool EntityExists() { return entity != nullptr; }

private:
	Entity* entity;
	void DeleteEntity() { entity = nullptr; }
};

class Entity
{
public:
	Entity() { entityHandle = std::make_shared<EntityHandle>(EntityHandle(this)); }
	~Entity() { entityHandle->DeleteEntity(); }

	Transform* ObjectTransform() { return &transform; }

	std::string MeshName() const { return meshName; }
	void MeshName(std::string meshName) { this->meshName = meshName; }

	std::string ColliderMeshName() const { return colliderMeshName; }
	void ColliderMeshName(std::string colliderMeshName) { this->colliderMeshName = colliderMeshName; }

	std::string MaterialName() const { return materialName; }
	void MaterialName(std::string materialName) { this->materialName = materialName; }

	bool IsBlock() const { return isBlock; }
	void IsBlock(bool isBlock) { this->isBlock = isBlock; }

	std::shared_ptr<EntityHandle> Handle() const { return entityHandle; }

private:
	Transform transform;
	std::string meshName;
	std::string materialName;
	std::string colliderMeshName;
	std::shared_ptr<EntityHandle> entityHandle;
	bool isBlock;
};

