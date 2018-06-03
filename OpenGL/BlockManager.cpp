#include "BlockManager.h"
#include "Entity.h"
#include "GUI.h"
#include "imgui.h"
#include "ApplicationContext.h"
#include "tinyxml2.h"

BlockManager::BlockManager(FileManager * fileManager)
{
	this->blockNames = fileManager->BlockNames();
	this->materialNames = fileManager->MaterialNames();
	selectedBlockIndex = 0;
	selectedMaterialIndex = 0;
	selectedColorIndex = 0;
	selectedBlockColorCount = 0;
	selectedColliderIndex = 0;
	selectedBlockPreviewDirty = true;
	selectedPreset = 0;

	strcpy_s(this->newPresetName, 32, "New Preset");

	LoadBrushes(fileManager);
	selectedBrushIndex = 0;
	placementMode = PlacementMode::Detail;
	brushAxisPlane = AxisPlane::XY;
}

void BlockManager::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();

	if (input->MouseOverGUIElement())
	{
		return;
	}

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
		BlockPreset blockPreset = GetBlockPresetAtPosition(glm::ivec3(0, 0, 0));
		context->ApplicationRenderer()->RenderModelPreview(blockPreset.meshName, blockPreset.colorIndex, blockPreset.colliderName, blockPreset.materialName);
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
		if (selectedPreset >= 0 && blockPresets.size() > 0)
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

		// Materials
		if (ImGui::SmallButton("-##material"))
		{
			this->SelectPreviousMaterial();
		}
		ImGui::SameLine();
		if (ImGui::SmallButton("+##material"))
		{
			this->SelectNextMaterial();
		}
		ImGui::SameLine();
		std::string selectedMaterialText = "Material: " + this->SelectedMaterialName();
		ImGui::Text(selectedMaterialText.c_str());

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
		if (GUI::SmallToggleButton(" XY", (int)AxisPlane::XY, selectedAxisPlane))
		{
			brushAxisPlane = AxisPlane::XY;
		}
		ImGui::SameLine(50.0f);
		if (GUI::SmallToggleButton(" XZ", (int)AxisPlane::XZ, selectedAxisPlane))
		{
			brushAxisPlane = AxisPlane::XZ;
		}
		ImGui::SameLine(100.0f);
		if (GUI::SmallToggleButton(" YZ", (int)AxisPlane::YZ, selectedAxisPlane))
		{
			brushAxisPlane = AxisPlane::YZ;
		}
		if (GUI::SmallToggleButton("-XY", (int)AxisPlane::XY_Neg, selectedAxisPlane))
		{
			brushAxisPlane = AxisPlane::XY_Neg;
		}
		ImGui::SameLine(50.0f);
		if (GUI::SmallToggleButton("-XZ", (int)AxisPlane::XZ_Neg, selectedAxisPlane))
		{
			brushAxisPlane = AxisPlane::XZ_Neg;
		}
		ImGui::SameLine(100.0f);
		if (GUI::SmallToggleButton("-YZ", (int)AxisPlane::YZ_Neg, selectedAxisPlane))
		{
			brushAxisPlane = AxisPlane::YZ_Neg;
		}


		brushAxisPlane = (AxisPlane)selectedAxisPlane;
	}

	// Preview Image
	ImTextureID texID = (ImTextureID)context->ApplicationRenderer()->ModelPreviewTextureID();
	ImGui::Image(texID, ImVec2(256, 256), ImVec2(1, 1), ImVec2(0, 0));

	ImGui::End();
}

void BlockManager::LoadBrushes(FileManager * fileManager)
{
	for (auto& brushPath : fileManager->BrushPaths())
	{
		tinyxml2::XMLDocument xmlDoc;
		xmlDoc.LoadFile(brushPath.second.c_str());

		Brush brush;
		auto brushNode = xmlDoc.FirstChildElement("Brush");
		auto brushNameNode = brushNode->FirstChildElement("Name");
		if (brushNameNode != nullptr)
		{
			brush.name = brushNameNode->GetText();
		}

		auto patternNode = brushNode->FirstChildElement("Pattern");
		if (patternNode != nullptr)
		{
			std::string patternText = patternNode->GetText();
			if (patternText == "Single")
			{
				brush.blockPattern = BlockPattern::Single;
			}
			else if (patternText == "Checker")
			{
				brush.blockPattern = BlockPattern::Checker;
			}
			else if (patternText == "TwoByTwo")
			{
				brush.blockPattern = BlockPattern::TwoByTwo;
			}
		}

		auto blockPresetsNode = brushNode->FirstChildElement("BlockPresets");
		for (auto* blockPresetNode = blockPresetsNode->FirstChildElement("BlockPreset"); blockPresetNode != nullptr; blockPresetNode = blockPresetNode->NextSiblingElement("BlockPreset"))
		{
			BlockPreset blockPreset;

			auto nameNode = blockPresetNode->FirstChildElement("Name");
			if (nameNode != nullptr)
			{
				blockPreset.name = nameNode->GetText();
			}

			auto meshNode = blockPresetNode->FirstChildElement("Mesh");
			if (meshNode != nullptr)
			{
				blockPreset.meshName = meshNode->GetText();
			}

			auto uvNode = blockPresetNode->FirstChildElement("UV");
			if (uvNode != nullptr)
			{
				blockPreset.colorIndex = uvNode->IntText(0);
			}

			auto colliderNode = blockPresetNode->FirstChildElement("Collider");
			if (colliderNode != nullptr)
			{
				blockPreset.colliderName = colliderNode->GetText();
			}

			auto materialNode = blockPresetNode->FirstChildElement("Material");
			if (materialNode != nullptr)
			{
				blockPreset.materialName = materialNode->GetText();
			}

			brush.blockPresets.push_back(blockPreset);
		}

		brushes.push_back(brush);
	}
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
		preset.materialName = SelectedMaterialName();
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

			switch (brushAxisPlane)
			{
			case AxisPlane::XY:
				if (position.y % 2 == 0)
					return activeBrush.blockPresets[abs(position.x) % 2];
				else
					return activeBrush.blockPresets[abs(position.x) % 2 + 2];
			case AxisPlane::XY_Neg:
				if (position.x % 2 == 0)
					return activeBrush.blockPresets[abs(position.y) % 2];
				else
					return activeBrush.blockPresets[abs(position.y) % 2 + 2];
			case AxisPlane::XZ:
				if (position.z % 2 == 0)
					return activeBrush.blockPresets[abs(position.x) % 2];
				else
					return activeBrush.blockPresets[abs(position.x) % 2 + 2];
			case AxisPlane::XZ_Neg:
				if (position.x % 2 == 0)
					return activeBrush.blockPresets[abs(position.z) % 2];
				else
					return activeBrush.blockPresets[abs(position.z) % 2 + 2];
			case AxisPlane::YZ:
				if (position.y % 2 == 0)
					return activeBrush.blockPresets[abs(position.z) % 2];
				else
					return activeBrush.blockPresets[abs(position.z) % 2 + 2];
			case AxisPlane::YZ_Neg:
				if (position.z % 2 == 0)
					return activeBrush.blockPresets[abs(position.y) % 2];
				else
					return activeBrush.blockPresets[abs(position.y) % 2 + 2];
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
	preset.materialName = SelectedMaterialName();
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
	selectedMaterialIndex = std::distance(materialNames.begin(), std::find(materialNames.begin(), materialNames.end(), blockPresets[selectedPreset].materialName));
	selectedBlockPreviewDirty = true;
}

void BlockManager::SelectPreviousPreset()
{
	int presetCount = blockPresets.size();
	selectedPreset = (((selectedPreset - 1) % presetCount) + presetCount) % presetCount;
	selectedBlockIndex = std::distance(blockNames.begin(), std::find(blockNames.begin(), blockNames.end(), blockPresets[selectedPreset].meshName));
	selectedColliderIndex = std::distance(blockNames.begin(), std::find(blockNames.begin(), blockNames.end(), blockPresets[selectedPreset].colliderName));
	selectedColorIndex = blockPresets[selectedPreset].colorIndex;
	selectedMaterialIndex = std::distance(materialNames.begin(), std::find(materialNames.begin(), materialNames.end(), blockPresets[selectedPreset].materialName));
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

void BlockManager::SelectNextMaterial()
{
	int materialCount = materialNames.size();
	selectedMaterialIndex = (((selectedMaterialIndex + 1) % materialCount) + materialCount) % materialCount;
	selectedBlockPreviewDirty = true;
	selectedPreset = -1;
}

void BlockManager::SelectPreviousMaterial()
{
	int materialCount = materialNames.size();
	selectedMaterialIndex = (((selectedMaterialIndex - 1) % materialCount) + materialCount) % materialCount;
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
