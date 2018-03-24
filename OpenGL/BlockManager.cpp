#include "BlockManager.h"
#include "Entity.h"
#include "GUI.h"
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

	BlockPreset greyCube;
	greyCube.meshName = "CubeBeveled";
	greyCube.colliderName = "Cube";
	greyCube.colorIndex = 1;

	BlockPreset blackCube;
	blackCube.meshName = "CubeBeveled";
	blackCube.colliderName = "Cube";
	blackCube.colorIndex = 2;

	Brush defaultBrush;
	defaultBrush.name = "Default";
	defaultBrush.blockPattern = BlockPattern::Single;
	defaultBrush.blockPresets.push_back(greyCube);

	Brush checkerBrush;
	checkerBrush.name = "Checker";
	checkerBrush.blockPattern = BlockPattern::Checker;
	checkerBrush.blockPresets.push_back(greyCube);
	checkerBrush.blockPresets.push_back(blackCube);

	BlockPreset wall0;
	wall0.meshName = "WallBrickGrey";
	wall0.colliderName = "Cube";
	wall0.colorIndex = 0;

	BlockPreset wall1;
	wall1.meshName = "WallBrickGrey";
	wall1.colliderName = "Cube";
	wall1.colorIndex = 1;

	BlockPreset wall2;
	wall2.meshName = "WallBrickGrey";
	wall2.colliderName = "Cube";
	wall2.colorIndex = 2;

	BlockPreset wall3;
	wall3.meshName = "WallBrickGrey";
	wall3.colliderName = "Cube";
	wall3.colorIndex = 3;

	Brush twoByTwoBrush;
	twoByTwoBrush.name = "Two By Two";
	twoByTwoBrush.blockPattern = BlockPattern::TwoByTwo;
	twoByTwoBrush.blockPresets.push_back(wall0);
	twoByTwoBrush.blockPresets.push_back(wall1);
	twoByTwoBrush.blockPresets.push_back(wall2);
	twoByTwoBrush.blockPresets.push_back(wall3);

	brushes.push_back(defaultBrush);
	brushes.push_back(checkerBrush);
	brushes.push_back(twoByTwoBrush);

	selectedBrushIndex = 0;
	placementMode = PlacementMode::Detail;
	brushAxisPlane = AxisPlane::XY;
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

	if (ImGui::SmallButton("-##placementmode"))
	{
		this->SelectPreviousPlacementMode();
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("+##placementmode"))
	{
		this->SelectNextPlacementMode();
	}
	ImGui::SameLine();
	std::string placementModeText = "Mode: " + PlacementModeText();
	ImGui::Text(placementModeText.c_str());

	if (placementMode == PlacementMode::Detail)
	{
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
	}
	else if (placementMode == PlacementMode::Brush)
	{
		// Brushes
		if (ImGui::SmallButton("-##brush"))
		{
			this->SelectPreviousBrush();
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("+##brush"))
		{
			this->SelectNextBrush();
		}
		ImGui::SameLine();
		std::string brushText = "Brush: " + brushes[selectedBrushIndex].name;
		ImGui::Text(brushText.c_str());

		int selectedAxisPlane = (int)brushAxisPlane;
		if (GUI::SmallToggleButton("XY", (int)AxisPlane::XY, selectedAxisPlane))
		{
			brushAxisPlane = AxisPlane::XY;
		}
		ImGui::SameLine();
		if (GUI::SmallToggleButton("XZ", (int)AxisPlane::XZ, selectedAxisPlane))
		{
			brushAxisPlane = AxisPlane::XZ;
		}
		ImGui::SameLine();
		if (GUI::SmallToggleButton("YZ", (int)AxisPlane::YZ, selectedAxisPlane))
		{
			brushAxisPlane = AxisPlane::YZ;
		}
		brushAxisPlane = (AxisPlane)selectedAxisPlane;
	}

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
		if (e->HasProperty(EntityProperty::Block))
		{
			blockMap[e->ObjectTransform()->GridPosition()] = e->Handle();
		}
	}

	return blockMap;
}

BlockPreset BlockManager::GetBlockPresetAtPosition(glm::ivec3 position)
{
	if (placementMode == PlacementMode::Detail)
	{
		BlockPreset preset;
		preset.meshName = SelectedBlockName();
		preset.colliderName = SelectedColliderName();
		preset.colorIndex = SelectedColorIndex();
		return preset;
	}
	else if (placementMode == PlacementMode::Brush)
	{
		Brush activeBrush = brushes[selectedBrushIndex];
		switch (activeBrush.blockPattern)
		{
		case BlockPattern::Single:
			return activeBrush.blockPresets[0];
		case BlockPattern::Checker:
			if ((position.x + position.y + position.z) % 2 == 0)
			{
				return activeBrush.blockPresets[0];
			}
			return activeBrush.blockPresets[1];
		case BlockPattern::TwoByTwo:
			if (brushAxisPlane == AxisPlane::XY)
			{
				if (position.y % 2 == 0)
				{
					return activeBrush.blockPresets[abs(position.x) % 2];
				}
				else
				{
					return activeBrush.blockPresets[abs(position.x) % 2 + 2];
				}
			}
			else if (brushAxisPlane == AxisPlane::XZ)
			{
				if (position.z % 2 == 0)
				{
					return activeBrush.blockPresets[abs(position.x) % 2];
				}
				else
				{
					return activeBrush.blockPresets[abs(position.x) % 2 + 2];
				}
			}
			else if (brushAxisPlane == AxisPlane::YZ)
			{
				if (position.y % 2 == 0)
				{
					return activeBrush.blockPresets[abs(position.z) % 2];
				}
				else
				{
					return activeBrush.blockPresets[abs(position.z) % 2 + 2];
				}
			}
		}
	}
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

void BlockManager::SelectNextBrush()
{
	int brushCount = brushes.size();
	selectedBrushIndex = (((selectedBrushIndex + 1) % brushCount) + brushCount) % brushCount;
	selectedBlockPreviewDirty = true;
	selectedPreset = -1;
}

void BlockManager::SelectPreviousBrush()
{
	int brushCount = brushes.size();
	selectedBrushIndex = (((selectedBrushIndex - 1) % brushCount) + brushCount) % brushCount;
	selectedBlockPreviewDirty = true;
	selectedPreset = -1;
}

void BlockManager::SelectNextPlacementMode()
{
	int modeCount = (int)PlacementMode::Size;
	placementMode = (PlacementMode)(((((int)placementMode + 1) % modeCount) + modeCount) % modeCount);
	selectedBlockPreviewDirty = true;
	selectedPreset = -1;
}

void BlockManager::SelectPreviousPlacementMode()
{
	int modeCount = (int)PlacementMode::Size;
	placementMode = (PlacementMode)(((((int)placementMode - 1) % modeCount) + modeCount) % modeCount);
	selectedBlockPreviewDirty = true;
	selectedPreset = -1;
}

std::string BlockManager::PlacementModeText() const
{
	if (placementMode == PlacementMode::Detail)
	{
		return "Detail";
	}
	else if (placementMode == PlacementMode::Brush)
	{
		return "Brush";
	}

	return "Other";
}
