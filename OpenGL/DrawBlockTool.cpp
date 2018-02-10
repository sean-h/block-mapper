#include "DrawBlockTool.h"
#include "ApplicationContext.h"
#include "glm\gtc\matrix_transform.hpp"
#include <iostream>

DrawBlockTool::DrawBlockTool(ApplicationContext* context)
{
	hoverBlock = context->ApplicationScene()->CreateEntity();
	this->RefreshHoverBlock(context);
}

void DrawBlockTool::Update(ApplicationContext* context)
{
	Scene* scene = context->ApplicationScene();
	Physics* physics = context->ApplicationPhysics();
	Input* input = context->ApplicationInput();
	Window* window = context->ApplicationWindow();

	glm::vec3 hoverBlockOriginalPosition = hoverBlock->ObjectTransform()->Position();
	hoverBlock->ObjectTransform()->Position(glm::vec3(10000, 10000, 10000));
	Camera* camera = scene->ActiveCamera();
	Transform* cameraTransform = camera->Owner()->ObjectTransform();

	glm::vec3 mouseDirection = camera->ScreenToWorldDirection(input->MouseX(), input->MouseY(), window->Width(), window->Height());
	RaycastHit hit = physics->Raycast(scene, cameraTransform->Position(), mouseDirection, 100.0f);
	if (hit.hit)
	{
		glm::vec3 newPos = hit.entity->ObjectTransform()->Position() + hit.normal;
		hoverBlock->ObjectTransform()->Position(newPos);
	}
	else
	{
		hoverBlock->ObjectTransform()->Position(hoverBlockOriginalPosition);
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
}

void DrawBlockTool::PlaceBlock(Scene* scene)
{
	Entity* newCube = scene->CreateEntity();
	newCube->ObjectTransform()->Position(hoverBlock->ObjectTransform()->Position());
	newCube->MeshName(hoverBlock->MeshName());
	newCube->ColliderMeshName(hoverBlock->ColliderMeshName());
	newCube->MaterialName("Solid");
}

void DrawBlockTool::RefreshHoverBlock(ApplicationContext * context)
{
	hoverBlock->MeshName(context->ApplicationBlockManager()->SelectedBlockName());
	hoverBlock->MaterialName("Hover");
	hoverBlock->ColliderMeshName("Cube");
}
