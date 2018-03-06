#include "DrawBlockTool.h"
#include "ApplicationContext.h"
#include "GUI.h"
#include <iostream>
#include "imgui.h"

DrawBlockTool::DrawBlockTool(ApplicationContext* context)
{
	hoverBlocks.push_back(context->ApplicationScene()->CreateEntity());
	this->RefreshHoverBlocks(context);

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
		this->RefreshHoverBlocks(context);
	}

	if (input->MouseOverGUIElement())
	{
		return;
	}

	if (input->GetKeyDown(Input::Keys::MOUSE_1))
	{
		buildState = BuildStates::PlaceLastBlock;
	}
	else if (input->GetKeyUp(Input::Keys::MOUSE_1))
	{
		if (buildState == BuildStates::Canceled)
		{
			buildState = BuildStates::PlaceFirstBlock;
		}
		else
		{
			buildState = BuildStates::Completed;
		}
	}
	else if (input->GetKeyDown(Input::Keys::MOUSE_2))
	{
		buildState = BuildStates::Canceled;
		this->RefreshHoverBlocks(context);
	}

	if (buildState == BuildStates::Completed)
	{
		this->PlaceBlocks(context->ApplicationScene());
	}

	glm::vec3 newBlockPos;
	if (this->GetHoverBlockPosition(context, newBlockPos))
	{
		if (buildState == BuildStates::PlaceFirstBlock)
		{
			if (drawStartPosition != newBlockPos)
			{
				drawStartPosition = newBlockPos;
				this->RefreshHoverBlocks(context);
			}
		}
		else if (buildState == BuildStates::PlaceLastBlock)
		{
			if (drawEndPosition != newBlockPos)
			{
				drawEndPosition = newBlockPos;
				this->RefreshHoverBlocks(context);
			}
		}
	}

	if (input->GetKeyDown(Input::Keys::KEY_E))
	{
		this->RotateHoverBlocks(context, glm::vec3(0.0f, 1.0f, 0.0f), -90.0f);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_Q))
	{
		this->RotateHoverBlocks(context, glm::vec3(0.0f, 1.0f, 0.0f), 90.0f);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_W))
	{
		this->RotateHoverBlocks(context, glm::vec3(1.0f, 0.0f, 0.0f), 90.0f);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_S))
	{
		this->RotateHoverBlocks(context, glm::vec3(1.0f, 0.0f, 0.0f), -90.0f);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_A))
	{
		this->RotateHoverBlocks(context, glm::vec3(0.0f, 0.0f, 1.0f), 90.0f);
	}
	else if (input->GetKeyDown(Input::Keys::KEY_D))
	{
		this->RotateHoverBlocks(context, glm::vec3(0.0f, 0.0f, 1.0f), -90.0f);
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

void DrawBlockTool::PlaceBlocks(Scene* scene)
{
	for (auto& hoverBlock : hoverBlocks)
	{
		auto newCubeHandle = scene->CreateEntity();
		Entity* newCube = newCubeHandle->TargetEntity();
		newCube->ObjectTransform()->Position(hoverBlock->TargetEntity()->ObjectTransform()->Position());
		newCube->ObjectTransform()->Rotation(hoverBlock->TargetEntity()->ObjectTransform()->RotationQuaternion());
		newCube->MeshName(hoverBlock->TargetEntity()->MeshName());
		newCube->ColliderMeshName("Cube");
		newCube->MaterialName("Solid");
		newCube->AddProperty("Block", "");
		newCube->MeshColorIndex(hoverBlock->TargetEntity()->MeshColorIndex());
		scene->RefreshEntityRenderData(newCubeHandle);
		scene->RefreshEntityCollisionData(newCubeHandle);
	}

	buildState = BuildStates::PlaceFirstBlock;
}

void DrawBlockTool::RefreshHoverBlocks(ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();
	this->ClearHoverBlocks(scene);

	if (buildState == BuildStates::Canceled)
	{
		return;
	}
	else if (buildState == BuildStates::PlaceFirstBlock)
	{
		hoverBlocks.push_back(context->ApplicationScene()->CreateEntity());
		hoverBlocks[0]->TargetEntity()->ObjectTransform()->Position(drawStartPosition);
		hoverBlocks[0]->TargetEntity()->ObjectTransform()->Rotation(drawRotation);
		hoverBlocks[0]->TargetEntity()->MeshName(context->ApplicationBlockManager()->SelectedBlockName());
		hoverBlocks[0]->TargetEntity()->MaterialName("Hover");
		hoverBlocks[0]->TargetEntity()->MeshColorIndex(context->ApplicationBlockManager()->SelectedColorIndex());
		scene->RefreshEntityRenderData(hoverBlocks[0]);
		return;
	}

	if (drawMode == DrawModes::Point)
	{
		auto hoverBlock = context->ApplicationScene()->CreateEntity();
		Entity* hoverBlockEntity = hoverBlock->TargetEntity();
		hoverBlockEntity->MaterialName("Hover");
		hoverBlockEntity->ObjectTransform()->Position(drawEndPosition);
		hoverBlockEntity->ObjectTransform()->Rotation(drawRotation);
		hoverBlockEntity->MeshName(context->ApplicationBlockManager()->SelectedBlockName());
		hoverBlockEntity->MeshColorIndex(context->ApplicationBlockManager()->SelectedColorIndex());
		this->hoverBlocks.push_back(hoverBlock);
		scene->RefreshEntityRenderData(hoverBlock);
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

					auto hoverBlock = context->ApplicationScene()->CreateEntity();
					Entity* hoverBlockEntity = hoverBlock->TargetEntity();

					hoverBlockEntity->MaterialName("Hover");
					hoverBlockEntity->ObjectTransform()->Position(blockPosition);
					hoverBlockEntity->ObjectTransform()->Rotation(drawRotation);
					hoverBlockEntity->MeshName(context->ApplicationBlockManager()->SelectedBlockName());
					hoverBlockEntity->MeshColorIndex(context->ApplicationBlockManager()->SelectedColorIndex());
					this->hoverBlocks.push_back(hoverBlock);
					scene->RefreshEntityRenderData(hoverBlock);
				}
			}
		}
	}
}

void DrawBlockTool::DisableTool(ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();
	this->ClearHoverBlocks(scene);
}

void DrawBlockTool::ClearHoverBlocks(Scene * scene)
{
	for (auto& hoverBlock : hoverBlocks)
	{
		scene->DestroyEntity(hoverBlock);
	}
	hoverBlocks.clear();
}

void DrawBlockTool::RotateHoverBlocks(ApplicationContext * context, glm::vec3 axis, float angleDegrees)
{
	drawRotation = glm::rotate(glm::quat(), glm::radians(angleDegrees), axis) * drawRotation;
	this->RefreshHoverBlocks(context);
}

bool DrawBlockTool::GetHoverBlockPosition(ApplicationContext * context, glm::vec3 & newBlockPosition)
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
