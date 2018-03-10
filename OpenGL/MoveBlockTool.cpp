#include "MoveBlockTool.h"
#include "Input.h"
#include "ApplicationContext.h"

MoveBlockTool::MoveBlockTool(ApplicationContext * context)
{
}

void MoveBlockTool::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();

	if (input->GetKeyDown(Input::Keys::KEY_W))
	{
		this->MoveBlocks(context, context->ApplicationScene()->ActiveCamera()->WorldAxisForward());
	}
	else if (input->GetKeyDown(Input::Keys::KEY_S))
	{
		this->MoveBlocks(context, -context->ApplicationScene()->ActiveCamera()->WorldAxisForward());
	}
	else if (input->GetKeyDown(Input::Keys::KEY_A))
	{
		this->MoveBlocks(context, -context->ApplicationScene()->ActiveCamera()->WorldAxisRight());
	}
	else if (input->GetKeyDown(Input::Keys::KEY_D))
	{
		this->MoveBlocks(context, context->ApplicationScene()->ActiveCamera()->WorldAxisRight());
	}
	else if (input->GetKeyDown(Input::Keys::KEY_Q))
	{
		this->MoveBlocks(context, glm::vec3(0.0f, -1.0f, 0.0f));
	}
	else if (input->GetKeyDown(Input::Keys::KEY_E))
	{
		this->MoveBlocks(context, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}

void MoveBlockTool::DrawGUI(ApplicationContext * context)
{
}

void MoveBlockTool::MoveBlocks(ApplicationContext * context, glm::vec3 direction)
{
	Scene* scene = context->ApplicationScene();
	for (auto& block : context->ApplicationEntitySelectionManager()->SelectedEntities())
	{
		if (block->EntityExists())
		{
			block->TargetEntity()->ObjectTransform()->Translate(direction);
			scene->RefreshEntityRenderModelMatrix(block);
			scene->RefreshEntityCollisionData(block);
		}
	}
}
