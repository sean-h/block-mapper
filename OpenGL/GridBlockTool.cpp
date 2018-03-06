#include "GridBlockTool.h"
#include "ApplicationContext.h"

GridBlockTool::GridBlockTool(ApplicationContext * context)
{
	this->firstBlockPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	this->lastBlockPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	drawShape = Shapes::HollowCube;
	buildStep = BuildSteps::PlaceFirstBlock;
	selectedBlockName = "";
	selectedBlockColorIndex = 0;

	this->PlaceHoverBlocks(context);
}

void GridBlockTool::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();
	BlockManager* blockManager = context->ApplicationBlockManager();

	if (selectedBlockName != blockManager->SelectedBlockName() || selectedBlockColorIndex != blockManager->SelectedColorIndex())
	{
		selectedBlockName = blockManager->SelectedBlockName();
		selectedBlockColorIndex = blockManager->SelectedColorIndex();
		this->PlaceHoverBlocks(context);
	}

	if (input->MouseOverGUIElement())
	{
		return;
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
	
	if (input->GetKeyDown(Input::Keys::MOUSE_1))
	{
		this->AdvanceBuildStep(context);
	}
}

void GridBlockTool::DrawGUI(ApplicationContext * context)
{
}

void GridBlockTool::RefreshHoverBlock(ApplicationContext * context)
{
}

void GridBlockTool::DisableTool(ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();
	for (auto& block : hoverBlocks)
	{
		scene->DestroyEntity(block);
	}
	hoverBlocks.clear();
}

void GridBlockTool::PlaceHoverBlocks(ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();

	for (int i = 0; i < this->hoverBlocks.size(); i++)
	{
		scene->DestroyEntity(hoverBlocks[i]);
	}
	this->hoverBlocks.clear();

	glm::vec3 buildVector = this->lastBlockPosition - this->firstBlockPosition;

	int endX = glm::abs((int)buildVector.x);
	int endY = glm::abs((int)buildVector.y);
	int endZ = glm::abs((int)buildVector.z);

	for (int x = 0; x <= endX; x++)
	{
		for (int y = 0; y <= endY; y++)
		{
			for (int z = 0; z <= endZ; z++)
			{
				if (drawShape == Shapes::FilledCube || x == 0 || x == endX || y == 0 || y == endY || z == 0 || z == endZ)
				{
					glm::vec3 blockPosition = glm::vec3(
						this->firstBlockPosition.x + (x * glm::sign(this->lastBlockPosition.x - this->firstBlockPosition.x)),
						this->firstBlockPosition.y + (y * glm::sign(this->lastBlockPosition.y - this->firstBlockPosition.y)),
						this->firstBlockPosition.z + (z * glm::sign(this->lastBlockPosition.z - this->firstBlockPosition.z)));

					auto hoverBlock = context->ApplicationScene()->CreateEntity();
					Entity* hoverBlockEntity = hoverBlock->TargetEntity();

					hoverBlockEntity->MaterialName("Hover");
					hoverBlockEntity->ObjectTransform()->Position(blockPosition);
					hoverBlockEntity->MeshName(context->ApplicationBlockManager()->SelectedBlockName());
					scene->RefreshEntityRenderData(hoverBlock);

					this->hoverBlocks.push_back(hoverBlock);
				}
			}
		}
	}
}

void GridBlockTool::MoveBlock(ApplicationContext * context, glm::vec3 moveDirection)
{
	if (buildStep == BuildSteps::PlaceFirstBlock)
	{
		this->firstBlockPosition += moveDirection;
		this->lastBlockPosition += moveDirection;
		
	}
	else if (buildStep == BuildSteps::PlaceLastBlock)
	{
		this->lastBlockPosition += moveDirection;
	}

	this->PlaceHoverBlocks(context);
}

void GridBlockTool::Apply(ApplicationContext * context)
{
	Scene* scene = context->ApplicationScene();
	for (auto& block : hoverBlocks)
	{
		block->TargetEntity()->MaterialName("Solid");
		block->TargetEntity()->AddProperty("Block", "");
		block->TargetEntity()->ColliderMeshName("Cube");
		scene->RefreshEntityRenderData(block);
		scene->RefreshEntityCollisionData(block);
	}
	this->hoverBlocks.clear();

	this->buildStep = BuildSteps::PlaceFirstBlock;
	this->firstBlockPosition = this->lastBlockPosition;
}

void GridBlockTool::AdvanceBuildStep(ApplicationContext * context)
{
	if (buildStep == BuildSteps::PlaceFirstBlock)
	{
		buildStep = BuildSteps::PlaceLastBlock;
	}
	else if (buildStep == BuildSteps::PlaceLastBlock)
	{
		this->Apply(context);
	}
}
