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

	int MeshColorIndex() const { return meshColorIndex; }
	void MeshColorIndex(int colorIndex) { this->meshColorIndex = colorIndex; }

	std::string ColliderMeshName() const { return colliderMeshName; }
	void ColliderMeshName(std::string colliderMeshName) { this->colliderMeshName = colliderMeshName; }

	std::string MaterialName() const { return materialName; }
	void MaterialName(std::string materialName) { this->materialName = materialName; }

	int RenderID() const { return renderID; }
	void RenderID(int id) { this->renderID = id; }

	unsigned int PhysicsID() const { return physicsID; }
	void PhysicsID(unsigned int id) { this->physicsID = id; }

	void AddProperty(std::string propertyName, std::string propertyValue) { this->properties[propertyName] = propertyValue; }
	void RemoveProperty(std::string propertyName);
	bool HasProperty(std::string propertyName) const { return properties.find(propertyName) != properties.end(); }
	std::string PropertyValue(std::string propertyName) { return properties[propertyName]; }
	std::unordered_map<std::string, std::string> Properties() const { return properties; }

	std::shared_ptr<EntityHandle> Handle() const { return entityHandle; }

private:
	int id;
	int renderID;
	unsigned int physicsID;
	Transform transform;
	std::string meshName;
	int meshColorIndex = 0;
	std::string materialName;
	std::string colliderMeshName;
	std::shared_ptr<EntityHandle> entityHandle;
	std::unordered_map<std::string, std::string> properties;
};

