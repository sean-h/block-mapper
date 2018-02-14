#include "DrawBlockTool.h"
#include "ApplicationContext.h"
#include "glm\gtc\matrix_transform.hpp"
#include <iostream>
#include "imgui.h"

DrawBlockTool::DrawBlockTool(ApplicationContext* context)
{
	hoverBlock = context->ApplicationScene()->CreateEntity();
	this->RefreshHoverBlock(context);

	gridPlane = context->ApplicationScene()->CreateEntity();
	Entity* gridPlaneEntity = gridPlane->TargetEntity();
	gridPlaneEntity->ObjectTransform()->Position(glm::vec3(0.0f, 0.0f, 0.0f));
	gridPlaneEntity->ObjectTransform()->Scale(glm::vec3(25.0f, 1.0f, 25.0f));
	gridPlaneEntity->MeshName("Plane");
	gridPlaneEntity->ColliderMeshName("Plane");
	gridPlaneEntity->MaterialName("Grid");

	gridPlaneBottom = context->ApplicationScene()->CreateEntity();
	Entity* gridPlaneBottomEntity = gridPlaneBottom->TargetEntity();
	gridPlaneBottomEntity->ObjectTransform()->Position(glm::vec3(0.0f, 0.0f, 0.0f));
	gridPlaneBottomEntity->ObjectTransform()->Scale(glm::vec3(25.0f, 1.0f, 25.0f));
	gridPlaneBottomEntity->MeshName("PlaneBottom");
	gridPlaneBottomEntity->ColliderMeshName("PlaneBottom");
	gridPlaneBottomEntity->MaterialName("Grid");
}

void DrawBlockTool::Update(ApplicationContext* context)
{
	Scene* scene = context->ApplicationScene();
	Physics* physics = context->ApplicationPhysics();
	Input* input = context->ApplicationInput();
	Window* window = context->ApplicationWindow();

	Entity* hoverBlockEntity = hoverBlock->TargetEntity();

	glm::vec3 hoverBlockOriginalPosition = hoverBlockEntity->ObjectTransform()->Position();
	hoverBlockEntity->ObjectTransform()->Position(glm::vec3(10000, 10000, 10000));
	Camera* camera = scene->ActiveCamera();
	Transform* cameraTransform = camera->Owner()->ObjectTransform();

	glm::vec3 mouseDirection = camera->ScreenToWorldDirection(input->MouseX(), input->MouseY(), window->Width(), window->Height());
	RaycastHit hit = physics->Raycast(scene, cameraTransform->Position(), mouseDirection, 100.0f);
	if (hit.hit)
	{
		if (hit.entity->ColliderMeshName() == "Plane" || hit.entity->ColliderMeshName() == "PlaneBottom")
		{
			glm::vec3 newPos = hit.point;
			if (hit.normal.y != 0.0f)
			{
				newPos.x = glm::round(newPos.x);
				newPos.y = hit.entity->ObjectTransform()->Position().y;
				newPos.z = glm::round(newPos.z);
			}
			hoverBlockEntity->ObjectTransform()->Position(newPos);
		}
		else
		{
			glm::vec3 newPos = hit.entity->ObjectTransform()->Position() + hit.normal;
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

	if (input->GetKeyDown(Input::Keys::KEY_EQUAL))
	{
		gridPlane->TargetEntity()->ObjectTransform()->Translate(glm::vec3(0.0f, 1.0f, 0.0f));
		gridPlaneBottom->TargetEntity()->ObjectTransform()->Translate(glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (input->GetKeyDown(Input::Keys::KEY_MINUS))
	{
		gridPlane->TargetEntity()->ObjectTransform()->Translate(glm::vec3(0.0f, -1.0f, 0.0f));
		gridPlaneBottom->TargetEntity()->ObjectTransform()->Translate(glm::vec3(0.0f, -1.0f, 0.0f));
	}

	if (input->GetKeyDown(Input::Keys::KEY_P))
	{
		context->ApplicationScene()->Export(context);
	}
}

void DrawBlockTool::DrawGUI(ApplicationContext * context)
{
	bool drawToolWindowOpen = true;
	ImGui::Begin("Draw", &drawToolWindowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	ImGui::SetWindowPos(ImVec2(10.0f, 250.0f));
	ImGui::SetWindowSize(ImVec2(150.0f, 200.0f));

	ImGui::Text("Draw");

	ImGui::End();
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
	scene->DestroyEntity(gridPlane);
	scene->DestroyEntity(gridPlaneBottom);

	hoverBlock.reset();
	gridPlane.reset();
	gridPlaneBottom.reset();
}
