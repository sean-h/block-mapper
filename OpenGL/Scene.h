#pragma once
#include "glm\glm.hpp"
#include "Entity.h"
#include "Component.h"
#include "Camera.h"
#include "Physics.h"
#include "tinyxml2.h"
#include <memory>
#include <vector>
#include <string>
#include <queue>

class ApplicationContext;

class Scene
{
public:
	Scene();
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);
	std::vector<std::unique_ptr<Entity>>& Entities() { return entities; }
	int EntityCount() { return entities.size(); }

	std::shared_ptr<EntityHandle> CreateEntity();
	void DestroyEntity(std::shared_ptr<EntityHandle> entityHandle);
	Component* AddComponentToEntity(Entity* entity, std::unique_ptr<Component> component);
	void DestroyComponent(Entity* entity, std::string componentType);
	Camera* ActiveCamera() { return camera; }
	void Export(ApplicationContext* context);
	std::string SceneName() const { return std::string(sceneName); }
	void SaveScene(ApplicationContext* context) const;
	bool SaveSceneWithOverwriteConfirmation(ApplicationContext* context, std::string confirmedSceneName) const;
	void LoadScene(ApplicationContext* context, std::string loadFilePath);

	void RefreshEntityRenderData(std::shared_ptr<EntityHandle> entityHandle);
	void RefreshEntityRenderModelMatrix(std::shared_ptr<EntityHandle> entityHandle);

	void RefreshEntityCollisionData(std::shared_ptr<EntityHandle> entityHandle);
	void RefreshEntityCollisionPosition(std::shared_ptr<EntityHandle> entityHandle);

	RaycastHit Raycast(Physics* physics, glm::vec3 origin, glm::vec3 direction, float distance);

private:
	void ClearScene();
	std::shared_ptr<EntityHandle> CreateEntity(int entityID);
	void CreateGridPlanes();

	std::vector<std::unique_ptr<Entity>> entities;
	int entityCounter;
	std::vector<std::unique_ptr<Component>> components;
	Camera* camera;
	char sceneName[64] = { 0 };

	std::shared_ptr<EntityHandle> gridPlane;
	std::shared_ptr<EntityHandle> gridPlaneBottom;
	int selectedGridPlane = 2;

	std::queue<std::shared_ptr<EntityHandle>> newRenderObjectsQueue;
	std::queue<std::shared_ptr<EntityHandle>> updatedRenderObjectsQueue;
	std::queue<int> destroyedRenderObjectsQueue;

	std::queue<std::shared_ptr<EntityHandle>> newPhysicsObjectsQueue;
	std::queue<std::shared_ptr<EntityHandle>> updatedPhysicsObjectsQueue;
	std::queue<unsigned int> destroyedPhysicsObjectsQueue;
};