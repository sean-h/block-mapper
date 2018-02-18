#include "SelectBlockTool.h"
#include "imgui.h"
#include "ApplicationContext.h"
#include "glm\glm.hpp"

SelectBlockTool::SelectBlockTool(ApplicationContext * context)
{
}

void SelectBlockTool::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();
	if (input->GetKeyDown(Input::Keys::MOUSE_1))
	{
		Window* window = context->ApplicationWindow();
		Scene* scene = context->ApplicationScene();
		Camera* camera = scene->ActiveCamera();
		Physics* physics = context->ApplicationPhysics();

		glm::vec3 mouseDirection = camera->ScreenToWorldDirection(input->MouseX(), input->MouseY(), window->Width(), window->Height());
		RaycastHit hit = physics->Raycast(scene, camera->Owner()->ObjectTransform()->Position(), mouseDirection, 100.0f);

		EntitySelectionManager* selectionManager = context->ApplicationEntitySelectionManager();
		if (!input->GetKey(Input::Keys::KEY_LEFT_SHIFT))
		{
			selectionManager->DeselectAll();
		}

		if (hit.hit)
		{
			selectionManager->SelectEntity(hit.entity->Handle());
		}
	}
}

void SelectBlockTool::DrawGUI(ApplicationContext * context)
{
	ImGui::Text("Select");
}
