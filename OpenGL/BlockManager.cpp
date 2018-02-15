#include "BlockManager.h"
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

	ImGui::SetWindowPos(ImVec2(10.0f, 500.0f));
	ImGui::SetWindowSize(ImVec2(275.0f, 300.0f));

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
