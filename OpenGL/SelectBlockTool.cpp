#include "SelectBlockTool.h"
#include "GUI.h"
#include "ApplicationContext.h"
#include "Entity.h"
#include "BlockManager.h"
#include "imgui.h"
#include "glm\glm.hpp"
#include <stack>
#include <set>

SelectBlockTool::SelectBlockTool(ApplicationContext * context)
{
	selectionMode = SelectionModes::Single;
	selectionModeIndex = 0;
}

void SelectBlockTool::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();

	if (input->MouseOverGUIElement())
	{
		return;
	}

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
			if (selectionMode == SelectionModes::Single)
			{
				this->SelectSingle(selectionManager, hit.entity);
			}
			else if (selectionMode == SelectionModes::Region)
			{
				this->SelectRegion(selectionManager, hit.entity, context->ApplicationBlockManager()->BlockPositionMap(scene));
			}
			else if (selectionMode == SelectionModes::Border)
			{
				this->SelectBorder(selectionManager, hit.entity, context->ApplicationBlockManager()->BlockPositionMap(scene));
			}
		}
	}
}

void SelectBlockTool::DrawGUI(ApplicationContext * context)
{
	ImGui::Text("Select");


	if (GUI::ToggleButton("Single", 0, selectionModeIndex))
	{
		selectionMode = SelectionModes::Single;
	}

	if (GUI::ToggleButton("Region", 1, selectionModeIndex))
	{
		selectionMode = SelectionModes::Region;
	}

	if (GUI::ToggleButton("Border", 2, selectionModeIndex))
	{
		selectionMode = SelectionModes::Border;
	}
}

void SelectBlockTool::SelectSingle(EntitySelectionManager * selectionManager, std::shared_ptr<EntityHandle> hitEntity)
{
	selectionManager->SelectEntity(hitEntity);
}

void SelectBlockTool::SelectRegion(EntitySelectionManager * selectionManager, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap)
{
	selectionManager->SelectEntity(hitEntity);
	glm::ivec3 gridPosition = hitEntity->TargetEntity()->ObjectTransform()->GridPosition();

	std::set<glm::ivec3> visited;
	std::stack<glm::ivec3> searchFrom;
	searchFrom.push(gridPosition);

	while (searchFrom.size() > 0)
	{
		auto adjacent = AdjacentPositions(searchFrom.top());
		searchFrom.pop();

		for (auto& pos : adjacent)
		{
			if (visited.find(pos) != visited.end())
			{
				continue;
			}

			visited.insert(pos);

			auto adjacentBlock = blockMap[pos];
			if (adjacentBlock)
			{
				selectionManager->SelectEntity(adjacentBlock);
				searchFrom.push(pos);
			}
		}
	}
}

void SelectBlockTool::SelectBorder(EntitySelectionManager * selectionManager, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap)
{
	glm::ivec3 gridPosition = hitEntity->TargetEntity()->ObjectTransform()->GridPosition();

	std::set<glm::ivec3> visited;
	std::stack<glm::ivec3> searchFrom;
	searchFrom.push(gridPosition);

	while (searchFrom.size() > 0)
	{
		glm::ivec3 currentPosition = searchFrom.top();
		searchFrom.pop();
		auto adjacent = AdjacentPositions(currentPosition);
		visited.insert(currentPosition);
		int adjacentBlockCount = 0;
		
		for (auto& pos : adjacent)
		{
			auto adjacentBlock = blockMap[pos];
			if (adjacentBlock)
			{
				adjacentBlockCount++;

				if (visited.find(pos) == visited.end())
				{
					searchFrom.push(pos);
				}
			}
		}

		if (adjacentBlockCount < 4)
		{
			selectionManager->SelectEntity(blockMap[currentPosition]);
		}
	}
}

std::vector<glm::ivec3> AdjacentPositions(glm::ivec3 position)
{
	std::vector<glm::ivec3> adjacent;
	adjacent.push_back(position + glm::ivec3(1, 0, 0));
	adjacent.push_back(position + glm::ivec3(-1, 0, 0));
	adjacent.push_back(position + glm::ivec3(0, 1, 0));
	adjacent.push_back(position + glm::ivec3(0, -1, 0));
	adjacent.push_back(position + glm::ivec3(0, 0, 1));
	adjacent.push_back(position + glm::ivec3(0, 0, -1));
	return adjacent;
}
