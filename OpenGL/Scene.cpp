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
	cubePositions.push_back(glm::vec3(1.0f, 0.2f, -1.0f));
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
}

void Scene::Update(ApplicationContext* context)
{
	Input* input = context->ApplicationInput();
	Time* time = context->ApplicationTime();

	for (auto &c : components)
	{
		c->Update(context);
	}

	/*camera->ProcessMouseMovement(input->MouseXDelta(), input->MouseYDelta(), GL_TRUE);

	if (input->GetKey(Input::Keys::KEY_W))
	{
		camera->ProcessKeyboard(Camera_Movement::FORWARD, time->DeltaTime());
	}
	if (input->GetKey(Input::Keys::KEY_S))
	{
		camera->ProcessKeyboard(Camera_Movement::BACKWARD, time->DeltaTime());
	}
	if (input->GetKey(Input::Keys::KEY_A))
	{
		camera->ProcessKeyboard(Camera_Movement::LEFT, time->DeltaTime());
	}
	if (input->GetKey(Input::Keys::KEY_D))
	{
		camera->ProcessKeyboard(Camera_Movement::RIGHT, time->DeltaTime());
	}*/
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
