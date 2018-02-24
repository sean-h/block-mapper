#include "DrawBlockTool.h"
#include "ApplicationContext.h"
#include "glm\gtc\matrix_transform.hpp"
#include <iostream>
#include "imgui.h"

DrawBlockTool::DrawBlockTool(ApplicationContext* context)
{
	hoverBlock = context->ApplicationScene()->CreateEntity();
	this->RefreshHoverBlock(context);
}

void DrawBlockTool::Update(ApplicationContext* context)
{
	Input* input = context->ApplicationInput();
	
	if (input->GetKeyDown(Input::Keys::KEY_PERIOD))
	{
		context->ApplicationBlockManager()->SelectNextBlock();
		this->RefreshHoverBlock(context);
	}
	if (input->GetKeyDown(Input::Keys::KEY_COMMA))
	{
		context->ApplicationBlockManager()->SelectPreviousBlock();
		this->RefreshHoverBlock(context);
	}

	if (input->MouseOverGUIElement())
	{
		return;
	}

	Window* window = context->ApplicationWindow();
	Scene* scene = context->ApplicationScene();
	Physics* physics = context->ApplicationPhysics();

	Entity* hoverBlockEntity = hoverBlock->TargetEntity();

	glm::vec3 hoverBlockOriginalPosition = hoverBlockEntity->ObjectTransform()->Position();
	hoverBlockEntity->ObjectTransform()->Position(glm::vec3(10000, 10000, 10000));
	Camera* camera = scene->ActiveCamera();
	Transform* cameraTransform = camera->Owner()->ObjectTransform();

	glm::vec3 mouseDirection = camera->ScreenToWorldDirection(input->MouseX(), input->MouseY(), window->Width(), window->Height());
	RaycastHit hit = physics->Raycast(scene, cameraTransform->Position(), mouseDirection, 100.0f);
	if (hit.hit)
	{
		Entity* hitEntity = hit.entity->TargetEntity();
		if (hitEntity->ColliderMeshName() == "Plane" || hitEntity->ColliderMeshName() == "PlaneBottom")
		{
			glm::vec3 newPos = hit.point;
			if (hit.normal.y != 0.0f)
			{
				newPos.x = glm::round(newPos.x);
				newPos.y = hitEntity->ObjectTransform()->Position().y;
				newPos.z = glm::round(newPos.z);
			}
			hoverBlockEntity->ObjectTransform()->Position(newPos);
		}
		else
		{
			glm::vec3 newPos = hitEntity->ObjectTransform()->Position() + hit.normal;
			context->ApplicationDebug()->LogToUI("Draw Position", newPos);
			hoverBlockEntity->ObjectTransform()->Position(newPos);
		}
	}
	else
	{
		hoverBlockEntity->ObjectTransform()->Position(hoverBlockOriginalPosition);
	}

	if (input->GetKeyDown(Input::Keys::MOUSE_1))
	{
		this->PlaceBlock(scene);
	}

	if (input->GetKeyDown(Input::Keys::KEY_E))
	{
		hoverBlockEntity->ObjectTransform()->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);
	}
	if (input->GetKeyDown(Input::Keys::KEY_Q))
	{
		hoverBlockEntity->ObjectTransform()->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
	}
	if (input->GetKeyDown(Input::Keys::KEY_W))
	{
		hoverBlockEntity->ObjectTransform()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	}
	if (input->GetKeyDown(Input::Keys::KEY_S))
	{
		hoverBlockEntity->ObjectTransform()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), -90.0f);
	}
	if (input->GetKeyDown(Input::Keys::KEY_A))
	{
		hoverBlockEntity->ObjectTransform()->Rotate(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f);
	}
	if (input->GetKeyDown(Input::Keys::KEY_D))
	{
		hoverBlockEntity->ObjectTransform()->Rotate(glm::vec3(0.0f, 0.0f, 1.0f), -90.0f);
	}
}

void DrawBlockTool::DrawGUI(ApplicationContext * context)
{
	ImGui::Text("Draw");
}

void DrawBlockTool::PlaceBlock(Scene* scene)
{
	auto newCubeHandle = scene->CreateEntity();
	Entity* newCube = newCubeHandle->TargetEntity();
	newCube->ObjectTransform()->Position(hoverBlock->TargetEntity()->ObjectTransform()->Position());
	newCube->ObjectTransform()->Rotation(hoverBlock->TargetEntity()->ObjectTransform()->Rotation());
	newCube->MeshName(hoverBlock->TargetEntity()->MeshName());
	newCube->ColliderMeshName(hoverBlock->TargetEntity()->ColliderMeshName());
	newCube->MaterialName("Solid");
	newCube->AddProperty("Block", "");
}

void DrawBlockTool::RefreshHoverBlock(ApplicationContext * context)
{
	hoverBlock->TargetEntity()->MeshName(context->ApplicationBlockManager()->SelectedBlockName());
	hoverBlock->TargetEntity()->MaterialName("Hover");
	hoverBlock->TargetEntity()->ColliderMeshName("Cube");
}

void DrawBlockTool::DisableTool(ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();
	scene->DestroyEntity(hoverBlock);

	hoverBlock.reset();
}
