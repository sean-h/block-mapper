#include "DrawBlockTool.h"
#include "ApplicationContext.h"
#include "GUI.h"
#include <iostream>
#include "imgui.h"

DrawBlockTool::DrawBlockTool(ApplicationContext* context)
{
	this->RefreshGhostBlocks(context);

	selectedDrawModeIndex = 0;
	drawMode = DrawModes::Point;
	buildState = BuildStates::PlaceFirstBlock;
	selectedBlockName = "";
	selectedBlockColorIndex = 0;
}

void DrawBlockTool::Update(ApplicationContext* context)
{
	Input* input = context->ApplicationInput();
	BlockManager* blockManager = context->ApplicationBlockManager();
	
	if (selectedBlockName != blockManager->SelectedBlockName() || selectedBlockColorIndex != blockManager->SelectedColorIndex())
	{
		selectedBlockName = blockManager->SelectedBlockName();
		selectedBlockColorIndex = blockManager->SelectedColorIndex();
		this->RefreshGhostBlocks(context);
	}

	if (input->MouseOverGUIElement())
	{
		return;
	}

	if (buildState == BuildStates::PlaceFirstBlock)
	{
		if (input->GetKeyDown(Input::Keys::MOUSE_1))
		{
			buildState = BuildStates::PlaceLastBlock;
		}

		glm::vec3 newBlockPos;
		if (this->GetNewGhostBlockPosition(context, newBlockPos))
		{
			if (drawStartPosition != newBlockPos)
			{
				drawStartPosition = newBlockPos;
				this->RefreshGhostBlocks(context);
			}
		}
	}
	else if (buildState == BuildStates::PlaceLastBlock)
	{
		if (input->GetKeyUp(Input::Keys::MOUSE_1))
		{
			buildState = BuildStates::Completed;
		}
		else if (input->GetKeyDown(Input::Keys::MOUSE_2))
		{
			buildState = BuildStates::Canceled;
			this->RefreshGhostBlocks(context);
		}

		glm::vec3 newBlockPos;
		if (this->GetNewGhostBlockPosition(context, newBlockPos))
		{
			if (drawEndPosition != newBlockPos)
			{
				drawEndPosition = newBlockPos;
				this->RefreshGhostBlocks(context);
			}
		}
	}
	else if (buildState == BuildStates::Completed)
	{
		this->Apply(context);
		buildState = BuildStates::PlaceFirstBlock;
	}
	else if (buildState == BuildStates::Canceled)
	{
		if (input->GetKeyUp(Input::Keys::MOUSE_1))
		{
			buildState = BuildStates::PlaceFirstBlock;
		}
	}

	if (input->GetKeyDown(Input::Keys::KEY_E))
	{
		this->RotateGhostBlocks(context, glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_Q))
	{
		this->RotateGhostBlocks(context, glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_W))
	{
		this->RotateGhostBlocks(context, glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_S))
	{
		this->RotateGhostBlocks(context, glm::vec3(1.0f, 0.0f, 0.0f), -90.0f);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_A))
	{
		this->RotateGhostBlocks(context, glm::vec3(0.0f, 0.0f, 1.0f), 90.0f);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_D))
	{
		this->RotateGhostBlocks(context, glm::vec3(0.0f, 0.0f, 1.0f), -90.0f);
	}
}

void DrawBlockTool::DrawGUI(ApplicationContext * context)
{
	ImGui::Text("Draw");

	if (GUI::ToggleButton("Point", 0, selectedDrawModeIndex))
	{
		drawMode = DrawModes::Point;
	}

	if (GUI::ToggleButton("Line", 1, selectedDrawModeIndex))
	{
		drawMode = DrawModes::Line;
	}

	if (GUI::ToggleButton("Rectangle", 2, selectedDrawModeIndex))
	{
		drawMode = DrawModes::Rectangle;
	}
}

void DrawBlockTool::RefreshGhostBlocks(ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();
	this->ClearGhostBlocks(scene);

	if (buildState == BuildStates::Canceled)
	{
		return;
	}
	else if (buildState == BuildStates::PlaceFirstBlock)
	{
		this->PlaceGhostBlock(context, drawStartPosition, drawRotation);
		return;
	}

	if (drawMode == DrawModes::Point)
	{
		this->PlaceGhostBlock(context, drawEndPosition, drawRotation);
	}
	else if (drawMode == DrawModes::Rectangle)
	{
		glm::vec3 buildVector = this->drawEndPosition - this->drawStartPosition;

		int endX = glm::abs((int)buildVector.x);
		int endY = glm::abs((int)buildVector.y);
		int endZ = glm::abs((int)buildVector.z);

		for (int x = 0; x <= endX; x++)
		{
			for (int y = 0; y <= endY; y++)
			{
				for (int z = 0; z <= endZ; z++)
				{
					glm::vec3 blockPosition = glm::vec3(
						this->drawStartPosition.x + (x * glm::sign(this->drawEndPosition.x - this->drawStartPosition.x)),
						this->drawStartPosition.y + (y * glm::sign(this->drawEndPosition.y - this->drawStartPosition.y)),
						this->drawStartPosition.z + (z * glm::sign(this->drawEndPosition.z - this->drawStartPosition.z)));
					this->PlaceGhostBlock(context, blockPosition, drawRotation);
				}
			}
		}
	}
}

void DrawBlockTool::RotateGhostBlocks(ApplicationContext * context, glm::vec3 axis, float angleDegrees)
{
	drawRotation = glm::rotate(glm::quat(), glm::radians(angleDegrees), axis) * drawRotation;
	this->RefreshGhostBlocks(context);
}

bool DrawBlockTool::GetNewGhostBlockPosition(ApplicationContext * context, glm::vec3 & newBlockPosition)
{
	Window* window = context->ApplicationWindow();
	Scene* scene = context->ApplicationScene();
	Physics* physics = context->ApplicationPhysics();
	Input* input = context->ApplicationInput();
	Camera* camera = scene->ActiveCamera();
	Transform* cameraTransform = camera->Owner()->ObjectTransform();

	glm::vec3 mouseDirection = camera->ScreenToWorldDirection(input->MouseX(), input->MouseY(), window->Width(), window->Height());
	RaycastHit hit = scene->Raycast(physics, cameraTransform->Position(), mouseDirection, 100.0f);
	if (hit.hit)
	{
		Entity* hitEntity = hit.entity->TargetEntity();
		glm::vec3 newPos;

		if (hitEntity->ColliderMeshName() == "Plane" || hitEntity->ColliderMeshName() == "PlaneBottom")
		{
			newPos = hit.point;
			if (hit.normal.y != 0.0f)
			{
				newPos.x = glm::round(newPos.x);
				newPos.y = hitEntity->ObjectTransform()->Position().y;
				newPos.z = glm::round(newPos.z);
			}
		}
		else
		{
			newPos = hitEntity->ObjectTransform()->Position() + hit.normal;
			context->ApplicationDebug()->LogToUI("Draw Position", newPos);
		}

		newBlockPosition = newPos;

		return true;
	}

	return false;
}
