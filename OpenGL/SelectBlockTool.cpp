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

SelectBlockTool::SelectBlockTool(ApplicationContext * context, SelectionModes selectionMode)
{
	this->selectionMode = selectionMode;
}

void SelectBlockTool::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();

	if (input->MouseOverGUIElement())
	{
		return;
	}

	if (input->GetKeyDown(Input::Keys::KEY_X))
	{
		EntitySelectionManager* selectionManager = context->ApplicationEntitySelectionManager();
		Scene* scene = context->ApplicationScene();
		for (auto& block : selectionManager->SelectedEntities())
		{
			scene->DestroyEntity(block);
		}
		selectionManager->DeselectAll(scene);
	}

	if (selectionMode == SelectionModes::Box)
	{
		if (input->GetKeyDown(Input::Keys::MOUSE_1))
		{
			selectBoxBegin = glm::vec2(input->MouseX(), input->MouseY());

			EntitySelectionManager* selectionManager = context->ApplicationEntitySelectionManager();
			if (!input->GetKey(Input::Keys::KEY_LEFT_SHIFT))
			{
				Scene* scene = context->ApplicationScene();
				selectionManager->DeselectAll(scene);
			}
		}
		
		if (input->GetKeyUp(Input::Keys::MOUSE_1))
		{
			selectBoxEnd = glm::vec2(input->MouseX(), input->MouseY());

			// Check if selection box begin and end are the same. If they are, move the end to make the frustum calculations work.
			if (selectBoxEnd.x == selectBoxBegin.x)
			{
				selectBoxEnd.x += 1.0f;
			}

			if (selectBoxEnd.y == selectBoxBegin.y)
			{
				selectBoxEnd.y += 1.0f;
			}

			Window* window = context->ApplicationWindow();
			Scene* scene = context->ApplicationScene();
			Camera* camera = scene->ActiveCamera();
			Physics* physics = context->ApplicationPhysics();
			float screenWidth = window->Width();
			float screenHeight = window->Height();

			float minX = glm::min(selectBoxBegin.x, selectBoxEnd.x);
			float maxX = glm::max(selectBoxBegin.x, selectBoxEnd.x);
			float minY = glm::min(selectBoxBegin.y, selectBoxEnd.y);
			float maxY = glm::max(selectBoxBegin.y, selectBoxEnd.y);

			Frustum frustum;
			frustum.cameraPosition = camera->Owner()->ObjectTransform()->Position();
			frustum.nearPlaneDistance = camera->NearClipDistance();
			frustum.nearPlanePosition = camera->Owner()->ObjectTransform()->Position() + (camera->NearClipDistance() * camera->Owner()->ObjectTransform()->Forward());
			frustum.nearPlaneNormal = -camera->Owner()->ObjectTransform()->Forward();
			frustum.farPlaneDistance = camera->FarClipDistance();
			frustum.topLeft = camera->ScreenToWorldDirection(minX, minY, screenWidth, screenHeight);
			frustum.topRight = camera->ScreenToWorldDirection(maxX, minY, screenWidth, screenHeight);
			frustum.bottomLeft = camera->ScreenToWorldDirection(minX, maxY, screenWidth, screenHeight);
			frustum.bottomRight = camera->ScreenToWorldDirection(maxX, maxY, screenWidth, screenHeight);

			EntitySelectionManager* selectionManager = context->ApplicationEntitySelectionManager();
			auto enclosedEntites = context->ApplicationPhysics()->FrustumIntersect(context->ApplicationDebug(), scene, frustum);
			for (auto& entity : enclosedEntites)
			{
				selectionManager->SelectEntity(scene, entity);
			}
		}
		return;
	}

	if (input->GetKeyDown(Input::Keys::MOUSE_1))
	{
		Window* window = context->ApplicationWindow();
		Scene* scene = context->ApplicationScene();
		Camera* camera = scene->ActiveCamera();
		Physics* physics = context->ApplicationPhysics();

		glm::vec3 mouseDirection = camera->ScreenToWorldDirection(input->MouseX(), input->MouseY(), window->Width(), window->Height());
		RaycastHit hit = scene->Raycast(physics, camera->Owner()->ObjectTransform()->Position(), mouseDirection, 100.0f);

		EntitySelectionManager* selectionManager = context->ApplicationEntitySelectionManager();
		if (!input->GetKey(Input::Keys::KEY_LEFT_SHIFT))
		{
			selectionManager->DeselectAll(scene);
		}

		if (hit.hit)
		{
			if (selectionMode == SelectionModes::Single)
			{
				this->SelectSingle(selectionManager, scene, hit.entity);
			}
			else if (selectionMode == SelectionModes::Region)
			{
				this->SelectRegion(selectionManager, scene, hit.entity, context->ApplicationBlockManager()->BlockPositionMap(scene));
			}
			else if (selectionMode == SelectionModes::Border)
			{
				this->SelectBorder(selectionManager, scene, hit.entity, context->ApplicationBlockManager()->BlockPositionMap(scene));
			}
			else if (selectionMode == SelectionModes::Plane)
			{
				this->SelectPlane(selectionManager, scene, hit.entity, context->ApplicationBlockManager()->BlockPositionMap(scene));
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

	if (GUI::ToggleButton("Box", 3, selectionModeIndex))
	{
		selectionMode = SelectionModes::Box;
	}

	if (GUI::ToggleButton("Plane", 4, selectionModeIndex))
	{
		selectionMode = SelectionModes::Plane;
	}
}

void SelectBlockTool::SelectSingle(EntitySelectionManager * selectionManager, Scene* scene, std::shared_ptr<EntityHandle> hitEntity)
{
	selectionManager->ToggleSelectEntity(scene, hitEntity);
}

void SelectBlockTool::SelectRegion(EntitySelectionManager * selectionManager, Scene* scene, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap)
{
	selectionManager->SelectEntity(scene, hitEntity);
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
				selectionManager->SelectEntity(scene, adjacentBlock);
				searchFrom.push(pos);
			}
		}
	}
}

void SelectBlockTool::SelectBorder(EntitySelectionManager * selectionManager, Scene* scene, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap)
{
	if (!hitEntity->TargetEntity()->HasProperty("Block"))
	{
		return;
	}

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
			selectionManager->SelectEntity(scene, blockMap[currentPosition]);
		}
	}
}

void SelectBlockTool::SelectPlane(EntitySelectionManager * selectionManager, Scene * scene, std::shared_ptr<EntityHandle> hitEntity, BlockMap blockMap)
{
	// Determine plane
	glm::ivec3 hitEntityGridPosition = hitEntity->TargetEntity()->ObjectTransform()->GridPosition();
	int xBias = 0;
	int yBias = 0;
	int zBias = 0;
	if (blockMap[hitEntityGridPosition + glm::ivec3(1, 0, 0)])
		xBias++;
	if (blockMap[hitEntityGridPosition + glm::ivec3(-1, 0, 0)])
		xBias++;
	if (blockMap[hitEntityGridPosition + glm::ivec3(0, 1, 0)])
		yBias++;
	if (blockMap[hitEntityGridPosition + glm::ivec3(0, -1, 0)])
		yBias++;
	if (blockMap[hitEntityGridPosition + glm::ivec3(0, 0, 1)])
		zBias++;
	if (blockMap[hitEntityGridPosition + glm::ivec3(0, 0, -1)])
		zBias++;

	int xyBias = xBias + yBias;
	int xzBias = xBias + zBias;
	int yzBias = yBias + zBias;

	Planes plane;

	if (xyBias >= xzBias && xyBias >= yzBias)
	{
		plane = Planes::XY;
	}
	else if (yzBias >= xyBias && yzBias >= xzBias)
	{
		plane = Planes::YZ;
	}
	else
	{
		plane = Planes::XZ;
	}

	std::set<glm::ivec3> visited;
	std::stack<glm::ivec3> searchFrom;
	searchFrom.push(hitEntityGridPosition);

	while (searchFrom.size() > 0)
	{
		auto adjacent = AdjacentPlanePositions(searchFrom.top(), plane);
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
				int neighbourCount = 0;
				auto neighbours = AdjacentPlanePositions(pos, plane);
				for (auto& n : neighbours)
				{
					if (blockMap[n])
					{
						neighbourCount++;
					}
				}

				if (neighbourCount >= 3)
				{
					selectionManager->SelectEntity(scene, adjacentBlock);
					searchFrom.push(pos);
				}
				
			}
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

std::vector<glm::ivec3> AdjacentPlanePositions(glm::ivec3 position, SelectBlockTool::Planes plane)
{
	std::vector<glm::ivec3> adjacent;

	if (plane == SelectBlockTool::Planes::XY)
	{
		adjacent.push_back(position + glm::ivec3(1, 0, 0));
		adjacent.push_back(position + glm::ivec3(-1, 0, 0));
		adjacent.push_back(position + glm::ivec3(0, 1, 0));
		adjacent.push_back(position + glm::ivec3(0, -1, 0));
	}
	else if (plane == SelectBlockTool::Planes::XZ)
	{
		adjacent.push_back(position + glm::ivec3(1, 0, 0));
		adjacent.push_back(position + glm::ivec3(-1, 0, 0));
		adjacent.push_back(position + glm::ivec3(0, 0, 1));
		adjacent.push_back(position + glm::ivec3(0, 0, -1));
	}
	else if (plane == SelectBlockTool::Planes::YZ)
	{
		adjacent.push_back(position + glm::ivec3(0, 1, 0));
		adjacent.push_back(position + glm::ivec3(0, -1, 0));
		adjacent.push_back(position + glm::ivec3(0, 0, 1));
		adjacent.push_back(position + glm::ivec3(0, 0, -1));
	}

	return adjacent;
}
