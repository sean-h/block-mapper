#include "PlaceBlockTool.h"
#include "ApplicationContext.h"
#include "glm\gtc\quaternion.hpp"

void PlaceBlockTool::Apply(ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();
	for (auto& block : ghostBlocks)
	{
		Entity* entity = block->TargetEntity();
		entity->MaterialName("Solid");
		entity->ColliderMeshName("Cube");
		entity->AddProperty("Block", "");
		scene->RefreshEntityRenderData(block);
		scene->RefreshEntityCollisionData(block);
	}

	ghostBlocks.clear();
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

	Entity* ghostBlock = ghostBlockHandle->TargetEntity();
	ghostBlock->ObjectTransform()->Position(position);
	ghostBlock->ObjectTransform()->Rotation(rotation);
	ghostBlock->MeshName(context->ApplicationBlockManager()->SelectedBlockName());
	ghostBlock->MeshColorIndex(context->ApplicationBlockManager()->SelectedColorIndex());
	ghostBlock->MaterialName("Hover");
	ghostBlock->AddProperty("Temporary", "");
	ghostBlock->RemoveProperty("Hidden");
	
	scene->RefreshEntityRenderData(ghostBlockHandle);

	this->ghostBlocks.push_back(ghostBlockHandle);
}

void PlaceBlockTool::ClearGhostBlocks(Scene* scene)
{
	for (auto& block : ghostBlocks)
	{
		block->TargetEntity()->AddProperty("Hidden", "");
		scene->RefreshEntityRenderData(block);
		ghostBlockPool.push(block);
	}

	ghostBlocks.clear();
}
