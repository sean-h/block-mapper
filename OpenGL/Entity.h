#pragma once
#include "Transform.h"
#include "Model.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <algorithm>

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
	Entity(int id);
	~Entity() { entityHandle->DeleteEntity(); }

	int ID() const { return id; }

	Transform* ObjectTransform() { return &transform; }

	std::string MeshName() const { return meshName; }
	void MeshName(std::string meshName) { this->meshName = meshName; }

	std::string ColliderMeshName() const { return colliderMeshName; }
	void ColliderMeshName(std::string colliderMeshName) { this->colliderMeshName = colliderMeshName; }

	std::string MaterialName() const { return materialName; }
	void MaterialName(std::string materialName) { this->materialName = materialName; }

	void AddProperty(std::string propertyName, std::string propertyValue) { this->properties[propertyName] = propertyValue; }
	void RemoveProperty(std::string propertyName);
	bool HasProperty(std::string propertyName) const { return properties.find(propertyName) != properties.end(); }
	std::string PropertyValue(std::string propertyName) { return properties[propertyName]; }
	std::unordered_map<std::string, std::string> Properties() const { return properties; }

	std::shared_ptr<EntityHandle> Handle() const { return entityHandle; }

private:
	int id;
	Transform transform;
	std::string meshName;
	std::string materialName;
	std::string colliderMeshName;
	std::shared_ptr<EntityHandle> entityHandle;
	std::unordered_map<std::string, std::string> properties;
};

