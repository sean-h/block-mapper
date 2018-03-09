#include "Scene.h"
#include "ApplicationContext.h"
#include "OrbitController.h"
#include "FPSController.h"
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
	cameraEntity->ObjectTransform()->Rotation(glm::vec3(45.0f, 0.0f, 0.0f));
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

	if (input->GetKeyDown(Input::Keys::KEY_H))
	{
		if (input->GetKey(Input::Keys::KEY_LEFT_ALT))
		{
			for (auto& entity : entities)
			{
				if (entity->HasProperty("Hidden"))
				{
					entity->RemoveProperty("Hidden");
					newRenderObjectsQueue.push(entity->Handle());
					RefreshEntityCollisionData(entity->Handle());
				}
			}
		}
		else
		{
			for (auto& entity : context->ApplicationEntitySelectionManager()->SelectedEntities())
			{
				if (entity->EntityExists())
				{
					entity->TargetEntity()->AddProperty("Hidden", "");
					destroyedRenderObjectsQueue.push(entity->TargetEntity()->RenderID());
					destroyedPhysicsObjectsQueue.push(entity->TargetEntity()->PhysicsID());
				}
			}

			context->ApplicationEntitySelectionManager()->DeselectAll(this);
		}
	}

	if (input->GetKeyDown(Input::Keys::KEY_F) && input->GetKey(Input::Keys::KEY_LEFT_SHIFT))
	{
		ActiveCamera()->SetFPSMode(context);
	}

	for (auto &c : components)
	{
		c->Update(context);
	}

	Renderer* renderer = context->ApplicationRenderer();

	while (newRenderObjectsQueue.size() > 0)
	{
		if (newRenderObjectsQueue.front()->EntityExists())
		{
			Entity* entity = newRenderObjectsQueue.front()->TargetEntity();
			if (entity->RenderID() == 0)
			{
				int renderID = renderer->AddRenderObject(entity->MeshName(), entity->MeshColorIndex(), entity->MaterialName(), entity->ObjectTransform()->Model());
				entity->RenderID(renderID);
			}
		}
		newRenderObjectsQueue.pop();
	}

	while (updatedRenderObjectsQueue.size() > 0)
	{
		if (updatedRenderObjectsQueue.front()->EntityExists())
		{
			Entity* entity = updatedRenderObjectsQueue.front()->TargetEntity();
			renderer->UpdateRenderObjectModelMatrix(entity->RenderID(), entity->ObjectTransform()->Model());
		}
		updatedRenderObjectsQueue.pop();
	}

	while (destroyedRenderObjectsQueue.size() > 0)
	{
		renderer->RemoveRenderObject(destroyedRenderObjectsQueue.front());
		destroyedRenderObjectsQueue.pop();
	}

	Physics* physics = context->ApplicationPhysics();

	while (newPhysicsObjectsQueue.size() > 0)
	{
		if (newPhysicsObjectsQueue.front()->EntityExists())
		{
			Entity* entity = newPhysicsObjectsQueue.front()->TargetEntity();
			unsigned int physicsID = physics->CreatePhysicsObject(entity->ObjectTransform()->Position(), entity->ObjectTransform()->Scale(), entity->ColliderMeshName());
			entity->PhysicsID(physicsID);
		}
		newPhysicsObjectsQueue.pop();
	}

	while (updatedPhysicsObjectsQueue.size() > 0)
	{
		if (updatedPhysicsObjectsQueue.front()->EntityExists())
		{
			Entity* entity = updatedPhysicsObjectsQueue.front()->TargetEntity();
			physics->UpdatePhysicsObjectPosition(entity->PhysicsID(), entity->ObjectTransform()->Position());
		}

		updatedPhysicsObjectsQueue.pop();
	}

	while (destroyedPhysicsObjectsQueue.size() > 0)
	{
		physics->DestroyPhysicsObject(destroyedPhysicsObjectsQueue.front());
		destroyedPhysicsObjectsQueue.pop();
	}
}

void Scene::DrawGUI(ApplicationContext * context)
{
	ImGui::Text("Scene");

	ImGui::InputText("Scene Name", this->sceneName, IM_ARRAYSIZE(this->sceneName));

	if (ImGui::Button("Export"))
	{
		this->Export(context);
	}

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
		updatedRenderObjectsQueue.push(gridPlane);
		updatedRenderObjectsQueue.push(gridPlaneBottom);
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
		updatedRenderObjectsQueue.push(gridPlane);
		updatedRenderObjectsQueue.push(gridPlaneBottom);
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
		updatedRenderObjectsQueue.push(gridPlane);
		updatedRenderObjectsQueue.push(gridPlaneBottom);
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

	if (entity->get()->RenderID() > 0)
	{
		destroyedRenderObjectsQueue.push(entity->get()->RenderID());
	}

	if (entity->get()->PhysicsID() != 0)
	{
		destroyedPhysicsObjectsQueue.push(entity->get()->PhysicsID());
	}

	entities.erase(entity);
}

Component * Scene::AddComponentToEntity(Entity * entity, std::unique_ptr<Component> component)
{
	this->components.push_back(std::move(component));
	this->components.back()->Owner(entity);
	this->components.back()->OnAttachToEntity();
	return this->components.back().get();
}

void Scene::DestroyComponent(Entity * entity, std::string componentType)
{
	int deleteIndex = -1;

	int i = 0;
	for (auto& component : components)
	{
		if (component->Owner() == entity && component->Type() == componentType)
		{
			deleteIndex = i;
			break;
		}
		i++;
	}

	if (deleteIndex != -1)
	{
		components.erase(components.begin() + deleteIndex);
	}
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
		if (entity->HasProperty("Temporary"))
		{
			continue;
		}

		auto entityNode = xmlDoc.NewElement("Entity");

		auto entityIDNode = xmlDoc.NewElement("ID");
		entityIDNode->SetText(std::to_string(entity->ID()).c_str());
		entityNode->InsertEndChild(entityIDNode);

		auto entityMeshNode = xmlDoc.NewElement("Mesh");
		entityMeshNode->SetText(entity->MeshName().c_str());
		entityNode->InsertEndChild(entityMeshNode);

		auto entityColorIndexNode = xmlDoc.NewElement("MeshColorIndex");
		entityColorIndexNode->SetText(std::to_string(entity->MeshColorIndex()).c_str());
		entityNode->InsertEndChild(entityColorIndexNode);

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
		
		auto entityPropertiesNode = xmlDoc.NewElement("Properties");
		for (auto& prop : entity->Properties())
		{
			auto propertyNode = xmlDoc.NewElement(prop.first.c_str());
			propertyNode->SetText(prop.second.c_str());
			entityPropertiesNode->InsertEndChild(propertyNode);
		}
		entityNode->InsertEndChild(entityPropertiesNode);

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

bool Scene::SaveSceneWithOverwriteConfirmation(ApplicationContext * context, std::string confirmedSceneName) const
{
	std::string filePath = context->ApplicationFileManager()->SaveFilePath() + this->sceneName + ".xml";
	bool saveFileExists = context->ApplicationFileManager()->FileExists(filePath);

	if (saveFileExists && this->sceneName != confirmedSceneName)
	{
		return false;
	}

	this->SaveScene(context);

	return true;
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

		auto colorIndexNode = entityNode->FirstChildElement("MeshColorIndex");
		std::string colorIndex = "0";
		if (colorIndexNode && colorIndexNode->GetText())
		{
			colorIndex = colorIndexNode->GetText();
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
		entity->TargetEntity()->MeshColorIndex(std::stoi(colorIndex));
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

		auto propertiesNode = entityNode->FirstChildElement("Properties");
		if (propertiesNode)
		{
			for (auto propertyNode = propertiesNode->FirstChild(); propertyNode != nullptr; propertyNode = propertyNode->NextSibling())
			{
				std::string propertyValue;
				if (propertyNode->ToElement()->GetText())
				{
					propertyValue = propertyNode->ToElement()->GetText();
				}

				entity->TargetEntity()->AddProperty(propertyNode->Value(), propertyValue);
			}
		}

		this->RefreshEntityRenderData(entity);
		this->RefreshEntityCollisionData(entity);
	}
}

void Scene::RefreshEntityRenderData(std::shared_ptr<EntityHandle> entityHandle)
{
	if (entityHandle->EntityExists())
	{
		Entity* entity = entityHandle->TargetEntity();

		// Delete old RenderObject
		if (entity->RenderID() != 0)
		{
			destroyedRenderObjectsQueue.push(entity->RenderID());
			entity->RenderID(0);
		}

		if (!entity->HasProperty("Hidden") && entity->MeshName() != "")
		{
			newRenderObjectsQueue.push(entityHandle);
		}
	}
}

void Scene::RefreshEntityRenderModelMatrix(std::shared_ptr<EntityHandle> entityHandle)
{
	if (entityHandle->EntityExists() && entityHandle->TargetEntity()->RenderID() > 0)
	{
		updatedRenderObjectsQueue.push(entityHandle);
	}
}

void Scene::RefreshEntityCollisionData(std::shared_ptr<EntityHandle> entityHandle)
{
	if (entityHandle->EntityExists())
	{
		Entity* entity = entityHandle->TargetEntity();

		if (entity->PhysicsID() != 0)
		{
			destroyedPhysicsObjectsQueue.push(entity->PhysicsID());
		}

		if (!entity->HasProperty("Hidden") && entity->ColliderMeshName() != "")
		{
			newPhysicsObjectsQueue.push(entityHandle);
		}
	}
}

void Scene::RefreshEntityCollisionPosition(std::shared_ptr<EntityHandle> entityHandle)
{
	if (entityHandle->EntityExists() && entityHandle->TargetEntity()->PhysicsID() != 0)
	{
		updatedPhysicsObjectsQueue.push(entityHandle);
	}
}

RaycastHit Scene::Raycast(Physics * physics, glm::vec3 origin, glm::vec3 direction, float distance)
{
	RaycastHit hit = physics->Raycast(this, origin, direction, distance);

	if (hit.hit)
	{
		for (auto& entity : entities)
		{
			if (entity->PhysicsID() == hit.physicsObjectID)
			{
				hit.entity = entity->Handle();
				break;
			}
		}
	}

	return hit;
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
	gridPlaneEntity->ObjectTransform()->Scale(glm::vec3(250.0f, 1.0f, 250.0f));
	gridPlaneEntity->MeshName("Plane");
	gridPlaneEntity->ColliderMeshName("Plane");
	gridPlaneEntity->MaterialName("Grid");
	newRenderObjectsQueue.push(gridPlane);
	RefreshEntityCollisionData(gridPlane);

	gridPlaneBottom = this->CreateEntity();
	Entity* gridPlaneBottomEntity = gridPlaneBottom->TargetEntity();
	gridPlaneBottomEntity->ObjectTransform()->Position(glm::vec3(0.0f, 0.0f, 0.0f));
	gridPlaneBottomEntity->ObjectTransform()->Scale(glm::vec3(250.0f, 1.0f, 250.0f));
	gridPlaneBottomEntity->MeshName("PlaneBottom");
	gridPlaneBottomEntity->ColliderMeshName("PlaneBottom");
	gridPlaneBottomEntity->MaterialName("Grid");
	newRenderObjectsQueue.push(gridPlaneBottom);
	RefreshEntityCollisionData(gridPlaneBottom);
}
