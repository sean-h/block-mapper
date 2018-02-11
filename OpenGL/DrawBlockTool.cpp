#include "DrawBlockTool.h"
#include "ApplicationContext.h"
#include "glm\gtc\matrix_transform.hpp"
#include <iostream>

DrawBlockTool::DrawBlockTool(ApplicationContext* context)
{
	hoverBlock = context->ApplicationScene()->CreateEntity();
	this->RefreshHoverBlock(context);

	gridPlane = context->ApplicationScene()->CreateEntity();
	gridPlane->ObjectTransform()->Position(glm::vec3(0.0f, 0.0f, 0.0f));
	gridPlane->ObjectTransform()->Scale(glm::vec3(25.0f, 1.0f, 25.0f));
	gridPlane->MeshName("Plane");
	gridPlane->ColliderMeshName("Plane");
	gridPlane->MaterialName("Grid");

	gridPlaneBottom = context->ApplicationScene()->CreateEntity();
	gridPlaneBottom->ObjectTransform()->Position(glm::vec3(0.0f, 0.0f, 0.0f));
	gridPlaneBottom->ObjectTransform()->Scale(glm::vec3(25.0f, 1.0f, 25.0f));
	gridPlaneBottom->MeshName("PlaneBottom");
	gridPlaneBottom->ColliderMeshName("PlaneBottom");
	gridPlaneBottom->MaterialName("Grid");
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
		if (hit.entity->ColliderMeshName() == "Plane" || hit.entity->ColliderMeshName() == "PlaneBottom")
		{
			glm::vec3 newPos = hit.point;
			if (hit.normal.y != 0.0f)
			{
				newPos.x = glm::round(newPos.x);
				newPos.y = hit.entity->ObjectTransform()->Position().y;
				newPos.z = glm::round(newPos.z);
			}
			hoverBlock->ObjectTransform()->Position(newPos);
		}
		else
		{
			glm::vec3 newPos = hit.entity->ObjectTransform()->Position() + hit.normal;
			hoverBlock->ObjectTransform()->Position(newPos);
		}
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

	if (input->GetKeyDown(Input::Keys::KEY_E))
	{
		hoverBlock->ObjectTransform()->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);
	}
	if (input->GetKeyDown(Input::Keys::KEY_Q))
	{
		hoverBlock->ObjectTransform()->Rotate(glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
	}
	if (input->GetKeyDown(Input::Keys::KEY_W))
	{
		hoverBlock->ObjectTransform()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	}
	if (input->GetKeyDown(Input::Keys::KEY_S))
	{
		hoverBlock->ObjectTransform()->Rotate(glm::vec3(1.0f, 0.0f, 0.0f), -90.0f);
	}
	if (input->GetKeyDown(Input::Keys::KEY_A))
	{
		hoverBlock->ObjectTransform()->Rotate(glm::vec3(0.0f, 0.0f, 1.0f), 90.0f);
	}
	if (input->GetKeyDown(Input::Keys::KEY_D))
	{
		hoverBlock->ObjectTransform()->Rotate(glm::vec3(0.0f, 0.0f, 1.0f), -90.0f);
	}

	if (input->GetKeyDown(Input::Keys::KEY_EQUAL))
	{
		gridPlane->ObjectTransform()->Translate(glm::vec3(0.0f, 1.0f, 0.0f));
		gridPlaneBottom->ObjectTransform()->Translate(glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else if (input->GetKeyDown(Input::Keys::KEY_MINUS))
	{
		gridPlane->ObjectTransform()->Translate(glm::vec3(0.0f, -1.0f, 0.0f));
		gridPlaneBottom->ObjectTransform()->Translate(glm::vec3(0.0f, -1.0f, 0.0f));
	}
}

void DrawBlockTool::PlaceBlock(Scene* scene)
{
	Entity* newCube = scene->CreateEntity();
	newCube->ObjectTransform()->Position(hoverBlock->ObjectTransform()->Position());
	newCube->ObjectTransform()->Rotation(hoverBlock->ObjectTransform()->Rotation());
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
