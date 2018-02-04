#include "DrawBlockTool.h"
#include <iostream>

void DrawBlockTool::Update(Scene * scene, Input * input, Physics * physics)
{
	if (input->GetKeyDown(Input::Keys::KEY_F))
	{
		Camera* camera = scene->ActiveCamera();
		RaycastHit hit = physics->Raycast(scene, camera->Position, camera->Front, 100.0f);
		if (hit.hit)
		{
			Entity* newCube = scene->CreateEntity();
			glm::vec3 newPos = hit.entity->ObjectTransform().Position() + hit.normal;
			newCube->ObjectTransform().Position(newPos);
		}
	}
}
