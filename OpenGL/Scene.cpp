#include "Scene.h"
#include "ApplicationContext.h"
#include "OrbitController.h"
#include "SceneExporter.h"
#include "GUI.h"
#include "imgui.h"
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

	this->CreateGridPlanes();
}

void Scene::Update(ApplicationContext* context)
{
	Input* input = context->ApplicationInput();
	if (input->GetKeyDown(Input::Keys::KEY_EQUAL))
	{
		if (gridPlane && gridPlaneBottom && gridPlane->EntityExists() && gridPlaneBottom->EntityExists())
		{
			gridPlane->TargetEntity()->ObjectTransform()->Translate(glm::vec3(0.0f, 1.0f, 0.0f));
			gridPlaneBottom->TargetEntity()->ObjectTransform()->Translate(glm::vec3(0.0f, 1.0f, 0.0f));
		}
	}
	else if (input->GetKeyDown(Input::Keys::KEY_MINUS))
	{
		if (gridPlane && gridPlaneBottom && gridPlane->EntityExists() && gridPlaneBottom->EntityExists())
		{
			gridPlane->TargetEntity()->ObjectTransform()->Translate(glm::vec3(0.0f, -1.0f, 0.0f));
			gridPlaneBottom->TargetEntity()->ObjectTransform()->Translate(glm::vec3(0.0f, -1.0f, 0.0f));
		}
	}

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

	ImGui::Text("Grid");
	if (GUI::ToggleButton("OFF", 0, this->selectedGridPlane, false))
	{
		if (gridPlane && gridPlaneBottom)
		{
			this->DestroyEntity(gridPlane);
			gridPlane = nullptr;
			this->DestroyEntity(gridPlaneBottom);
			gridPlaneBottom = nullptr;
		}
	}
	ImGui::SameLine();
	if (GUI::ToggleButton("XY", 1, this->selectedGridPlane, false))
	{
		if (gridPlane == nullptr)
		{
			this->CreateGridPlanes();
		}
		gridPlane->TargetEntity()->ObjectTransform()->Rotation(glm::vec3(90.0f, 0.0f, 0.0f));
		gridPlaneBottom->TargetEntity()->ObjectTransform()->Rotation(glm::vec3(90.0f, 0.0f, 0.0f));
	}
	ImGui::SameLine();
	if (GUI::ToggleButton("XZ", 2, this->selectedGridPlane, false))
	{
		if (gridPlane == nullptr)
		{
			this->CreateGridPlanes();
		}
		gridPlane->TargetEntity()->ObjectTransform()->Rotation(glm::vec3(0.0f, 0.0f, 0.0f));
		gridPlaneBottom->TargetEntity()->ObjectTransform()->Rotation(glm::vec3(0.0f, 0.0f, 0.0f));
	}
	ImGui::SameLine();
	if (GUI::ToggleButton("YZ", 3, this->selectedGridPlane, false))
	{
		if (gridPlane == nullptr)
		{
			this->CreateGridPlanes();
		}
		gridPlane->TargetEntity()->ObjectTransform()->Rotation(glm::vec3(0.0f, 0.0f, 90.0f));
		gridPlaneBottom->TargetEntity()->ObjectTransform()->Rotation(glm::vec3(0.0f, 0.0f, 90.0f));
	}
	ImGui::SameLine();
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

void Scene::LoadScene(ApplicationContext * context, std::string loadFilePath)
{
	tinyxml2::XMLDocument xmlDoc;
	xmlDoc.LoadFile(loadFilePath.c_str());

	//this->ClearScene();

	auto sceneNode = xmlDoc.FirstChild();

	auto sceneNameNode = sceneNode->FirstChildElement("SceneName");
	strcpy_s(this->sceneName, 64, sceneNameNode->GetText());

	auto entitiesNode = sceneNode->FirstChildElement("Entities");
	
	for (auto entityNode = entitiesNode->FirstChild(); entityNode != nullptr; entityNode = entityNode->NextSibling())
	{
		int entityID = std::stoi(entityNode->FirstChildElement("ID")->GetText());

		auto meshNode = entityNode->FirstChildElement("Mesh");
		std::string mesh;
		if (meshNode && meshNode->GetText())
		{
			mesh = meshNode->GetText();
		}
		
		auto materialNode = entityNode->FirstChildElement("Material");
		std::string material;
		if (materialNode && materialNode->GetText())
		{
			material = materialNode->GetText();
		}

		auto colliderNode = entityNode->FirstChildElement("Collider");
		std::string collider;
		if (colliderNode && colliderNode->GetText())
		{
			collider = colliderNode->GetText();
		}

		auto transformNode = entityNode->FirstChildElement("Transform");
		auto positionNode = transformNode->FirstChildElement("Position");
		auto rotationNode = transformNode->FirstChildElement("Rotation");
		auto scaleNode = transformNode->FirstChildElement("Scale");

		auto entity = this->CreateEntity(entityID);
		entity->TargetEntity()->MeshName(mesh);
		entity->TargetEntity()->MaterialName(material);
		entity->TargetEntity()->ColliderMeshName(collider);
		entity->TargetEntity()->ObjectTransform()->Position(glm::vec3(positionNode->FloatAttribute("X"), positionNode->FloatAttribute("Y"), positionNode->FloatAttribute("Z")));

		glm::quat quat;
		quat.x = rotationNode->FloatAttribute("X");
		quat.y = rotationNode->FloatAttribute("Y");
		quat.z = rotationNode->FloatAttribute("Z");
		quat.w = rotationNode->FloatAttribute("W");
		entity->TargetEntity()->ObjectTransform()->Rotation(quat);

		entity->TargetEntity()->ObjectTransform()->Scale(glm::vec3(scaleNode->FloatAttribute("X"), scaleNode->FloatAttribute("Y"), scaleNode->FloatAttribute("Z")));
	}
}

void Scene::ClearScene()
{
	this->entities.clear();
	this->components.clear();
	this->camera = nullptr;
	this->entityCounter = 0;
}

std::shared_ptr<EntityHandle> Scene::CreateEntity(int entityID)
{
	if (entityID > entityCounter)
	{
		entityCounter = entityID;
	}

	std::unique_ptr<Entity> entity(new Entity(entityID));
	this->entities.push_back(std::move(entity));
	return this->entities.back()->Handle();
}

void Scene::CreateGridPlanes()
{
	gridPlane = this->CreateEntity();
	Entity* gridPlaneEntity = gridPlane->TargetEntity();
	gridPlaneEntity->ObjectTransform()->Position(glm::vec3(0.0f, 0.0f, 0.0f));
	gridPlaneEntity->ObjectTransform()->Scale(glm::vec3(25.0f, 1.0f, 25.0f));
	gridPlaneEntity->MeshName("Plane");
	gridPlaneEntity->ColliderMeshName("Plane");
	gridPlaneEntity->MaterialName("Grid");

	gridPlaneBottom = this->CreateEntity();
	Entity* gridPlaneBottomEntity = gridPlaneBottom->TargetEntity();
	gridPlaneBottomEntity->ObjectTransform()->Position(glm::vec3(0.0f, 0.0f, 0.0f));
	gridPlaneBottomEntity->ObjectTransform()->Scale(glm::vec3(25.0f, 1.0f, 25.0f));
	gridPlaneBottomEntity->MeshName("PlaneBottom");
	gridPlaneBottomEntity->ColliderMeshName("PlaneBottom");
	gridPlaneBottomEntity->MaterialName("Grid");
}
