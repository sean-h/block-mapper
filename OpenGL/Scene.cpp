#include "Scene.h"
#include "ApplicationContext.h"
#include "OrbitController.h"

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
	cubePositions.push_back(glm::vec3(1.0f, 0.0f, -1.0f));
	cubePositions.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));

	for (auto p : cubePositions)
	{
		Entity* entity = this->CreateEntity();
		entity->ObjectTransform()->Position(p);
		entity->MeshName("Cube");
		entity->ColliderMeshName("Cube");
		entity->MaterialName("Solid");
	}

	Entity* cameraEntity = this->CreateEntity();
	cameraEntity->ObjectTransform()->Position(glm::vec3(0.0f, 0.0f, -10.0f));
	cameraEntity->ObjectTransform()->Rotation(glm::vec3(0.0f, 0.0f, 0.0f));
	AddComponentToEntity(cameraEntity, std::unique_ptr<Component>(new OrbitController()));
	camera = (Camera*)AddComponentToEntity(cameraEntity, std::unique_ptr<Component>(new Camera()));

	Entity* planeEntity = this->CreateEntity();
	planeEntity->ObjectTransform()->Position(glm::vec3(0.0f, -0.5f, 0.0f));
	planeEntity->ObjectTransform()->Scale(glm::vec3(100.0f, 1.0f, 100.0f));
	planeEntity->MeshName("Plane");
	planeEntity->ColliderMeshName("Cube");
	planeEntity->MaterialName("Grid");

	Entity* planeEntityBottom = this->CreateEntity();
	planeEntityBottom->ObjectTransform()->Position(glm::vec3(0.0f, -0.5f, 0.0f));
	planeEntityBottom->ObjectTransform()->Rotation(glm::vec3(180.0f, 0.0f, 0.0f));
	planeEntityBottom->ObjectTransform()->Scale(glm::vec3(100.0f, 1.0f, 100.0f));
	planeEntityBottom->MeshName("Plane");
	planeEntityBottom->ColliderMeshName("Cube");
	planeEntityBottom->MaterialName("Grid");
}

void Scene::Update(ApplicationContext* context)
{
	for (auto &c : components)
	{
		c->Update(context);
	}
}

Entity * Scene::CreateEntity()
{
	std::unique_ptr<Entity> entity(new Entity());
	this->entities.push_back(std::move(entity));
	return this->entities.back().get();
}

Component * Scene::AddComponentToEntity(Entity * entity, std::unique_ptr<Component> component)
{
	this->components.push_back(std::move(component));
	this->components.back()->Owner(entity);
	return this->components.back().get();
}
