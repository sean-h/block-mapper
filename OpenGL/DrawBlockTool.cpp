#include "DrawBlockTool.h"
#include "ApplicationContext.h"
#include <iostream>

DrawBlockTool::DrawBlockTool(ApplicationContext* context)
{
	hoverBlock = context->ApplicationScene()->CreateEntity();
}

void DrawBlockTool::Update(ApplicationContext* context)
{
	Scene* scene = context->ApplicationScene();
	Physics* physics = context->ApplicationPhysics();
	Input* input = context->ApplicationInput();

	glm::vec3 hoverBlockOriginalPosition = hoverBlock->ObjectTransform()->Position();
	hoverBlock->ObjectTransform()->Position(glm::vec3(10000, 10000, 10000));
	Camera* camera = scene->ActiveCamera();
	Transform* cameraTransform = camera->Owner()->ObjectTransform();
	RaycastHit hit = physics->Raycast(scene, cameraTransform->Position(), cameraTransform->Forward(), 100.0f);
	if (hit.hit)
	{
		glm::vec3 newPos = hit.entity->ObjectTransform()->Position() + hit.normal;
		hoverBlock->ObjectTransform()->Position(newPos);
	}
	else
	{
		hoverBlock->ObjectTransform()->Position(hoverBlockOriginalPosition);
	}

	if (input->GetKeyDown(Input::Keys::KEY_F))
	{
		Entity* newCube = scene->CreateEntity();
		newCube->ObjectTransform()->Position(hoverBlock->ObjectTransform()->Position());
	}
}
