#include "Scene.h"
#include "ApplicationContext.h"
#include "OrbitController.h"
#include "SceneExporter.h"
#include "imgui.h"
#include "tinyxml2.h"
#include <string>

Scene::Scene()
{
	entityCounter = 0;
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
	std::unique_ptr<Entity> entity(new Entity(++entityCounter));
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

void Scene::SaveScene(ApplicationContext * context) const
{
	tinyxml2::XMLDocument xmlDoc;

	auto sceneNode = xmlDoc.NewElement("Scene");
	xmlDoc.InsertFirstChild(sceneNode);

	auto sceneNameNode = xmlDoc.NewElement("SceneName");
	sceneNameNode->SetText(this->sceneName);
	sceneNode->InsertFirstChild(sceneNameNode);

	auto entitiesNode = xmlDoc.NewElement("Entities");
	for (auto& entity : this->entities)
	{
		auto entityNode = xmlDoc.NewElement("Entity");

		auto entityIDNode = xmlDoc.NewElement("ID");
		entityIDNode->SetText(std::to_string(entity->ID()).c_str());
		entityNode->InsertEndChild(entityIDNode);

		auto entityMeshNode = xmlDoc.NewElement("Mesh");
		entityMeshNode->SetText(entity->MeshName().c_str());
		entityNode->InsertEndChild(entityMeshNode);

		auto entityMaterialNode = xmlDoc.NewElement("Material");
		entityMaterialNode->SetText(entity->MaterialName().c_str());
		entityNode->InsertEndChild(entityMaterialNode);

		auto entityColliderNode = xmlDoc.NewElement("Collider");
		entityColliderNode->SetText(entity->ColliderMeshName().c_str());
		entityNode->InsertEndChild(entityColliderNode);

		auto entityTransformNode = xmlDoc.NewElement("Transform");

		auto entityTransformPositionNode = xmlDoc.NewElement("Position");
		entityTransformPositionNode->SetAttribute("X", entity->ObjectTransform()->Position().x);
		entityTransformPositionNode->SetAttribute("Y", entity->ObjectTransform()->Position().y);
		entityTransformPositionNode->SetAttribute("Z", entity->ObjectTransform()->Position().z);
		entityTransformNode->InsertEndChild(entityTransformPositionNode);

		auto entityTransformRotationNode = xmlDoc.NewElement("Rotation");
		entityTransformRotationNode->SetAttribute("X", entity->ObjectTransform()->RotationQuaternion().x);
		entityTransformRotationNode->SetAttribute("Y", entity->ObjectTransform()->RotationQuaternion().y);
		entityTransformRotationNode->SetAttribute("Z", entity->ObjectTransform()->RotationQuaternion().z);
		entityTransformRotationNode->SetAttribute("W", entity->ObjectTransform()->RotationQuaternion().w);
		entityTransformNode->InsertEndChild(entityTransformRotationNode);

		auto entityTransformScaleNode = xmlDoc.NewElement("Scale");
		entityTransformScaleNode->SetAttribute("X", entity->ObjectTransform()->Scale().x);
		entityTransformScaleNode->SetAttribute("Y", entity->ObjectTransform()->Scale().y);
		entityTransformScaleNode->SetAttribute("Z", entity->ObjectTransform()->Scale().z);
		entityTransformNode->InsertEndChild(entityTransformScaleNode);

		entityNode->InsertEndChild(entityTransformNode);

		entitiesNode->InsertEndChild(entityNode);
	}
	sceneNode->InsertEndChild(entitiesNode);

	auto componentsNode = xmlDoc.NewElement("Components");
	for (auto& component : components)
	{
		auto componentNode = xmlDoc.NewElement("Component");

		componentNode->SetAttribute("Type", component->Type().c_str());

		auto ownerNode = xmlDoc.NewElement("Owner");
		ownerNode->SetText(std::to_string(component->Owner()->ID()).c_str());
		componentNode->InsertEndChild(ownerNode);

		auto dataMap = component->Serialize();
		for (auto& dataPair : dataMap)
		{
			auto dataNode = xmlDoc.NewElement(dataPair.first.c_str());
			dataNode->SetText(dataPair.second.c_str());
			componentNode->InsertEndChild(dataNode);
		}

		componentsNode->InsertEndChild(componentNode);
	}
	sceneNode->InsertEndChild(componentsNode);

	std::string filePath = context->ApplicationFileManager()->SaveFilePath() + this->sceneName + ".xml";
	xmlDoc.SaveFile(filePath.c_str());
}
