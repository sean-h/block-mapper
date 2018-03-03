#include "ExtrudeBlockTool.h"
#include "ApplicationContext.h"

ExtrudeBlockTool::ExtrudeBlockTool(ApplicationContext * context)
{
	for (auto& selectedBlock : context->ApplicationEntitySelectionManager()->SelectedEntities())
	{
		if (selectedBlock->EntityExists())
		{
			extrudeFromPoints.push_back(selectedBlock->TargetEntity()->ObjectTransform()->Position());
		}
	}

	extrudeDirection = glm::vec3(0.0f, 1.0f, 0.0f);
	extrudeDistance = 0;
	selectedBlockName = "";
	selectedBlockColorIndex = 0;
	this->MoveBlock(context, glm::vec3(0.0f, 1.0f, 0.0f));
}

void ExtrudeBlockTool::Update(ApplicationContext * context)
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
		this->Apply(context);
	}

	if (input->GetKeyDown(Input::Keys::KEY_W))
	{
		this->MoveBlock(context, context->ApplicationScene()->ActiveCamera()->WorldAxisForward());
	}
	else if (input->GetKeyDown(Input::Keys::KEY_S))
	{
		this->MoveBlock(context, -context->ApplicationScene()->ActiveCamera()->WorldAxisForward());
	}
	else if (input->GetKeyDown(Input::Keys::KEY_A))
	{
		this->MoveBlock(context, -context->ApplicationScene()->ActiveCamera()->WorldAxisRight());
	}
	else if (input->GetKeyDown(Input::Keys::KEY_D))
	{
		this->MoveBlock(context, context->ApplicationScene()->ActiveCamera()->WorldAxisRight());
	}
	else if (input->GetKeyDown(Input::Keys::KEY_Q))
	{
		this->MoveBlock(context, glm::vec3(0.0f, -1.0f, 0.0f));
	}
	else if (input->GetKeyDown(Input::Keys::KEY_E))
	{
		this->MoveBlock(context, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void ExtrudeBlockTool::DrawGUI(ApplicationContext * context)
{
}

void ExtrudeBlockTool::DisableTool(ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();
	for (auto& block : hoverBlocks)
	{
		scene->DestroyEntity(block);
	}
	hoverBlocks.clear();
}

void ExtrudeBlockTool::MoveBlock(ApplicationContext * context, glm::vec3 direction)
{
	if (direction == extrudeDirection)
	{
		extrudeDistance++;
	}
	else if (direction == -extrudeDirection)
	{
		extrudeDistance--;
		if (extrudeDistance == 0)
		{
			extrudeDirection *= -1;
			extrudeDistance = 1;
		}
	}
	else
	{
		extrudeDirection = direction;
		extrudeDistance = 1;
	}

	this->RefreshHoverBlocks(context);
}

void ExtrudeBlockTool::Apply(ApplicationContext* context)
{
	Scene* scene = context->ApplicationScene();
	for (auto& block : hoverBlocks)
	{
		block->TargetEntity()->MaterialName("Solid");
		block->TargetEntity()->AddProperty("Block", "");
		scene->RefreshEntityRenderData(block);
	}
	this->hoverBlocks.clear();
}

void ExtrudeBlockTool::RefreshHoverBlocks(ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();
	for (auto& block : hoverBlocks)
	{
		scene->DestroyEntity(block);
	}
	hoverBlocks.clear();

	for (int i = 1; i <= extrudeDistance; i++)
	{
		for (auto& point : extrudeFromPoints)
		{
			auto hoverBlock = scene->CreateEntity();
			Entity* hoverBlockEntity = hoverBlock->TargetEntity();
			hoverBlockEntity->ObjectTransform()->Position(point + extrudeDirection * (float)i);
			hoverBlockEntity->MaterialName("Hover");
			hoverBlockEntity->ColliderMeshName("Cube");
			hoverBlockEntity->MeshName(context->ApplicationBlockManager()->SelectedBlockName());
			hoverBlockEntity->MeshColorIndex(context->ApplicationBlockManager()->SelectedColorIndex());
			scene->RefreshEntityRenderData(hoverBlock);
			hoverBlocks.push_back(hoverBlock);
		}
	}
}
