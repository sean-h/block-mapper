#include "BlockManager.h"
#include "Entity.h"
#include "imgui.h"
#include "ApplicationContext.h"

BlockManager::BlockManager(FileManager * fileManager)
{
	this->blockNames = fileManager->BlockNames();
	selectedBlockIndex = 0;
	selectedColorIndex = 0;
	selectedBlockColorCount = 0;
	selectedColliderIndex = 0;
	selectedBlockPreviewDirty = true;

	strcpy_s(this->newPresetName, 32, "New Preset");
}

void BlockManager::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();

	if (input->GetKeyDown(Input::Keys::KEY_PERIOD))
	{
		context->ApplicationBlockManager()->SelectNextBlock();
	}
	if (input->GetKeyDown(Input::Keys::KEY_COMMA))
	{
		context->ApplicationBlockManager()->SelectPreviousBlock();
	}
	if (input->GetKeyDown(Input::Keys::KEY_APOSTROPHE))
	{
		context->ApplicationBlockManager()->SelectNextColorIndex();
	}
	if (input->GetKeyDown(Input::Keys::KEY_SEMICOLON))
	{
		context->ApplicationBlockManager()->SelectPreviousColorIndex();
	}
}

void BlockManager::DrawGUI(ApplicationContext * context)
{
	if (selectedBlockPreviewDirty)
	{
		context->ApplicationRenderer()->RenderModelPreview(this->SelectedBlockName(), this->selectedColorIndex, this->SelectedColliderName());
		selectedBlockPreviewDirty = false;
		selectedBlockColorCount = context->ApplicationRenderer()->ModelUVIndexCount(SelectedBlockName());
	}

	bool drawBlockWindowOpen = true;
	ImGui::Begin("Blocks", &drawBlockWindowOpen, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

	GUILocation windowLocation = context->ApplicationGUIManager()->WindowLocation("BlockManager");
	ImGui::SetWindowPos(ImVec2(windowLocation.xPosition, windowLocation.yPosition));
	ImGui::SetWindowSize(ImVec2(windowLocation.width, windowLocation.height));

	// Presets
	if (ImGui::SmallButton("-##preset"))
	{
		this->SelectPreviousPreset();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+##preset"))
	{
		this->SelectNextPreset();
	}
	ImGui::SameLine();
	std::string selectedPresetText = "";
	if (selectedPreset >= 0)
	{
		selectedPresetText = "Preset: " + this->blockPresets[selectedPreset].name;
		ImGui::Text(selectedPresetText.c_str());
	}
	else
	{
		ImGui::PushItemWidth(160.0f);
		ImGui::InputText("", this->newPresetName, IM_ARRAYSIZE(this->newPresetName));
		ImGui::SameLine();
		if (ImGui::Button("Create"))
		{
			CreatePreset();
		}
	}
	

	// Meshes
	if (ImGui::SmallButton("-##mesh"))
	{
		this->SelectPreviousBlock();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+##mesh"))
	{
		this->SelectNextBlock();
	}
	ImGui::SameLine();
	std::string selectedBlockText = "Mesh: " + this->SelectedBlockName() + " (" + std::to_string(this->selectedColorIndex) + ")";
	ImGui::Text(selectedBlockText.c_str());

	// Colliders
	if (ImGui::SmallButton("-##collider"))
	{
		this->SelectPreviousCollider();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+##collider"))
	{
		this->SelectNextCollider();
	}
	ImGui::SameLine();
	std::string colliderText = "Collider: " + this->SelectedColliderName();
	ImGui::Text(colliderText.c_str());

	// Color Index
	if (ImGui::SmallButton("-##color"))
	{
		this->SelectPreviousColorIndex();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+##color"))
	{
		this->SelectNextColorIndex();
	}
	ImGui::SameLine();
	std::string colorText = "Color: " + std::to_string(this->SelectedColorIndex());
	ImGui::Text(colorText.c_str());

	// Preview Image
	ImTextureID texID = (ImTextureID)context->ApplicationRenderer()->ModelPreviewTextureID();
	ImGui::Image(texID, ImVec2(256, 256), ImVec2(1, 1), ImVec2(0, 0));

	ImGui::End();
}

void BlockManager::SelectNextBlock()
{
	int blockCount = blockNames.size();
	selectedBlockIndex = (((selectedBlockIndex + 1) % blockCount) + blockCount) % blockCount;
	selectedBlockPreviewDirty = true;
	selectedColorIndex = 0;
	selectedPreset = -1;
}

void BlockManager::SelectPreviousBlock()
{
	int blockCount = blockNames.size();
	selectedBlockIndex = (((selectedBlockIndex - 1) % blockCount) + blockCount) % blockCount;
	selectedBlockPreviewDirty = true;
	selectedColorIndex = 0;
	selectedPreset = -1;
}

void BlockManager::SelectNextColorIndex()
{
	selectedColorIndex = (((selectedColorIndex + 1) % selectedBlockColorCount) + selectedBlockColorCount) % selectedBlockColorCount;
	selectedBlockPreviewDirty = true;
	selectedPreset = -1;
}

void BlockManager::SelectPreviousColorIndex()
{
	selectedColorIndex = (((selectedColorIndex - 1) % selectedBlockColorCount) + selectedBlockColorCount) % selectedBlockColorCount;
	selectedBlockPreviewDirty = true;
	selectedPreset = -1;
}

void BlockManager::SelectNextCollider()
{
	int blockCount = blockNames.size();
	selectedColliderIndex = (((selectedColliderIndex + 1) % blockCount) + blockCount) % blockCount;
	selectedBlockPreviewDirty = true;
	selectedPreset = -1;
}

void BlockManager::SelectPreviousCollider()
{
	int blockCount = blockNames.size();
	selectedColliderIndex = (((selectedColliderIndex - 1) % blockCount) + blockCount) % blockCount;
	selectedBlockPreviewDirty = true;
	selectedPreset = -1;
}

BlockMap BlockManager::BlockPositionMap(Scene * scene)
{
	BlockMap blockMap;

	for (auto& e : scene->Entities())
	{
		if (e->HasProperty("Block"))
		{
			blockMap[e->ObjectTransform()->GridPosition()] = e->Handle();
		}
	}

	return blockMap;
}

void BlockManager::CreatePreset()
{
	BlockPreset preset;
	preset.name = newPresetName;
	preset.meshName = blockNames[selectedBlockIndex];
	preset.colliderName = blockNames[selectedColliderIndex];
	preset.colorIndex = selectedColorIndex;
	blockPresets.push_back(preset);
	selectedPreset = blockPresets.size() - 1;
}

void BlockManager::SelectNextPreset()
{
	int presetCount = blockPresets.size();
	selectedPreset = (((selectedPreset + 1) % presetCount) + presetCount) % presetCount;
	selectedBlockIndex = std::distance(blockNames.begin(), std::find(blockNames.begin(), blockNames.end(), blockPresets[selectedPreset].meshName));
	selectedColliderIndex = std::distance(blockNames.begin(), std::find(blockNames.begin(), blockNames.end(), blockPresets[selectedPreset].colliderName));
	selectedColorIndex = blockPresets[selectedPreset].colorIndex;
	selectedBlockPreviewDirty = true;
}

void BlockManager::SelectPreviousPreset()
{
	int presetCount = blockPresets.size();
	selectedPreset = (((selectedPreset - 1) % presetCount) + presetCount) % presetCount;
	selectedBlockIndex = std::distance(blockNames.begin(), std::find(blockNames.begin(), blockNames.end(), blockPresets[selectedPreset].meshName));
	selectedColliderIndex = std::distance(blockNames.begin(), std::find(blockNames.begin(), blockNames.end(), blockPresets[selectedPreset].colliderName));
	selectedColorIndex = blockPresets[selectedPreset].colorIndex;
	selectedBlockPreviewDirty = true;
}
