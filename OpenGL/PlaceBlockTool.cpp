#include "PlaceBlockTool.h"
#include "ApplicationContext.h"
#include "UndoAction.h"
#include "glm\gtc\quaternion.hpp"

void PlaceBlockTool::Apply(ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();
	std::unique_ptr<UndoAction> undoAction(new UndoAction());
	undoAction->action = UndoAction::PlaceEntities();

	for (auto& block : ghostBlocks)
	{
		Entity* entity = block->TargetEntity();

		glm::vec3 blockPosition = entity->ObjectTransform()->GridPosition();
		BlockPreset blockPreset = context->ApplicationBlockManager()->GetBlockPresetAtPosition(blockPosition);

		entity->MaterialName(blockPreset.materialName);
		entity->ColliderMeshName(blockPreset.colliderName);
		entity->AddProperty(EntityProperty::Block, "");
		entity->RemoveProperty(EntityProperty::Temporary);
		scene->RefreshEntityRenderData(block);
		scene->RefreshEntityCollisionData(block);

		std::get<UndoAction::PlaceEntities>(undoAction->action).entityIDs.push_back(entity->ID());
	}

	ghostBlocks.clear();
	context->ApplicationUndoManager()->PushUndoAction(std::move(undoAction));
}

void PlaceBlockTool::DisableTool(ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();

	for (auto& block : ghostBlocks)
	{
		scene->DestroyEntity(block);
	}
	ghostBlocks.clear();

	while (ghostBlockPool.size() > 0)
	{
		scene->DestroyEntity(ghostBlockPool.top());
		ghostBlockPool.pop();
	}
}

void PlaceBlockTool::PlaceGhostBlock(ApplicationContext * context, glm::vec3 position, glm::quat rotation)
{
	Scene* scene = context->ApplicationScene();

	std::shared_ptr<EntityHandle> ghostBlockHandle = nullptr;
		
	if (ghostBlockPool.size() > 0)
	{
		ghostBlockHandle = ghostBlockPool.top();
		ghostBlockPool.pop();
	}
	else
	{
		ghostBlockHandle = scene->CreateEntity();
	}

	glm::ivec3 blockPosition(glm::round(position.x), glm::round(position.y), glm::round(position.z));
	BlockPreset blockPreset = context->ApplicationBlockManager()->GetBlockPresetAtPosition(blockPosition);

	Entity* ghostBlock = ghostBlockHandle->TargetEntity();
	ghostBlock->ObjectTransform()->Position(position);
	ghostBlock->ObjectTransform()->Rotation(rotation);
	ghostBlock->MeshName(blockPreset.meshName);
	ghostBlock->MeshColorIndex(blockPreset.colorIndex);
	ghostBlock->MaterialName("Hover");
	ghostBlock->AddProperty(EntityProperty::Temporary, "");
	ghostBlock->RemoveProperty(EntityProperty::Hidden);
	
	scene->RefreshEntityRenderData(ghostBlockHandle);

	this->ghostBlocks.push_back(ghostBlockHandle);
}

void PlaceBlockTool::ClearGhostBlocks(Scene* scene)
{
	for (auto& block : ghostBlocks)
	{
		block->TargetEntity()->AddProperty(EntityProperty::Hidden, "");
		scene->RefreshEntityRenderData(block);
		ghostBlockPool.push(block);
	}

	ghostBlocks.clear();
}
