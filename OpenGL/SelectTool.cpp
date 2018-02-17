#include "SelectTool.h"
#include "imgui.h"
#include "ApplicationContext.h"
#include "glm\glm.hpp"

SelectTool::SelectTool(ApplicationContext * context)
{
}

void SelectTool::Update(ApplicationContext * context)
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
		if (hit.hit)
		{
			EntitySelectionManager* selectionManager = context->ApplicationEntitySelectionManager();

			if (!input->GetKey(Input::Keys::KEY_LEFT_SHIFT))
			{
				selectionManager->DeselectAll();
			}

			selectionManager->SelectEntity(hit.entity->Handle());
		}
	}
}

void SelectTool::DrawGUI(ApplicationContext * context)
{
	ImGui::Text("Select");
}
