#include "BlockManager.h"
#include "Entity.h"
#include "imgui.h"
#include "ApplicationContext.h"

BlockManager::BlockManager(FileManager * fileManager)
{
	this->blockNames = fileManager->BlockNames();
	selectedBlockIndex = 0;
	selectedBlockPreviewDirty = true;
}

void BlockManager::DrawGUI(ApplicationContext * context)
{
	if (selectedBlockPreviewDirty)
	{
		context->ApplicationRenderer()->RenderModelPreview(this->SelectedBlockName());
		selectedBlockPreviewDirty = false;
	}

	bool drawBlockWindowOpen = true;
	ImGui::Begin("Blocks", &drawBlockWindowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

	GUILocation windowLocation = context->ApplicationGUIManager()->WindowLocation("BlockManager");
	ImGui::SetWindowPos(ImVec2(windowLocation.xPosition, windowLocation.yPosition));
	ImGui::SetWindowSize(ImVec2(windowLocation.width, windowLocation.height));

	ImGui::Text(this->SelectedBlockName().c_str());

	ImTextureID texID = (ImTextureID)context->ApplicationRenderer()->ModelPreviewTextureID();
	ImGui::Image(texID, ImVec2(256, 256), ImVec2(1, 1), ImVec2(0, 0));

	ImGui::End();
}

void BlockManager::SelectNextBlock()
{
	selectedBlockIndex = (selectedBlockIndex + 1) % blockNames.size();
	selectedBlockPreviewDirty = true;
}

void BlockManager::SelectPreviousBlock()
{
	selectedBlockIndex = (selectedBlockIndex - 1) % blockNames.size();
	selectedBlockPreviewDirty = true;
}

BlockMap BlockManager::BlockPositionMap(Scene * scene)
{
	BlockMap blockMap;

	for (auto& e : scene->Entities())
	{
		if (e->IsBlock())
		{
			blockMap[e->ObjectTransform()->GridPosition()] = e->Handle();
		}
	}

	return blockMap;
}
