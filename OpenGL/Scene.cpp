#include "Scene.h"
#include "ApplicationContext.h"
#include "OrbitController.h"
#include "SceneExporter.h"
#include "imgui.h"
#include <string>

Scene::Scene()
{
	auto cameraEntityHandle = this->CreateEntity();
	Entity* cameraEntity = cameraEntityHandle->TargetEntity();
	cameraEntity->ObjectTransform()->Position(glm::vec3(0.0f, 20.0f, -20.0f));
	cameraEntity->ObjectTransform()->Rotation(glm::vec3(0.0f, 0.0f, 0.0f));
	AddComponentToEntity(cameraEntity, std::unique_ptr<Component>(new OrbitController()));
	camera = (Camera*)AddComponentToEntity(cameraEntity, std::unique_ptr<Component>(new Camera()));
	strcpy_s(this->sceneName, 64, "Scene");
}

void Scene::Update(ApplicationContext* context)
{
	for (auto &c : components)
	{
		c->Update(context);
	}
}

void Scene::DrawGUI(ApplicationContext * context)
{
	ImGui::Text("Scene");

	ImGui::InputText("Scene Name", this->sceneName, IM_ARRAYSIZE(this->sceneName));

	std::string entityCount = "Entity Count: " + std::to_string(entities.size());
	ImGui::Text(entityCount.c_str());
}

std::shared_ptr<EntityHandle> Scene::CreateEntity()
{
	std::unique_ptr<Entity> entity(new Entity());
	this->entities.push_back(std::move(entity));
	return this->entities.back()->Handle();
}

void Scene::DestroyEntity(std::shared_ptr<EntityHandle> entityHandle)
{
	auto entity = std::find_if(entities.begin(), entities.end(), [&entityHandle](const std::unique_ptr<Entity>& entityObj) { return entityObj.get() == entityHandle->TargetEntity(); });
	entities.erase(entity);
}

Component * Scene::AddComponentToEntity(Entity * entity, std::unique_ptr<Component> component)
{
	this->components.push_back(std::move(component));
	this->components.back()->Owner(entity);
	this->components.back()->OnAttachToEntity();
	return this->components.back().get();
}

void Scene::Export(ApplicationContext * context)
{
	SceneExporter exporter;
	exporter.Export(context);
}
