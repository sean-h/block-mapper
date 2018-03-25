#pragma once
#include "FileManager.h"
#include "glm\glm.hpp"
#include <vector>

class ApplicationContext;
class EntityHandle;
class Scene;

struct ivec3_hash
{
	size_t operator()(const glm::ivec3& k)const
	{
		return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
	}

	bool operator()(const glm::ivec3& a, const glm::ivec3& b)const
	{
		return a.x == b.x && a.y == b.y && a.z == b.z;
	}
};

typedef std::unordered_map<glm::ivec3, std::shared_ptr<EntityHandle>, ivec3_hash, ivec3_hash> BlockMap;

enum class BlockPattern
{
	Single,
	Checker,
	TwoByTwo,
};

struct BlockPreset
{
	std::string name;
	std::string meshName;
	int colorIndex;
	std::string colliderName;
	std::string materialName;
};

struct Brush
{
	std::string name;
	BlockPattern blockPattern;
	std::vector<BlockPreset> blockPresets;
};

class BlockManager
{
public:
	BlockManager(FileManager* fileManager);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);
	std::string SelectedBlockName() const { return blockNames[selectedBlockIndex]; }
	int SelectedColorIndex() const { return selectedColorIndex; }
	std::string SelectedColliderName() const { return blockNames[selectedColliderIndex]; }
	std::string SelectedMaterialName() const { return materialNames[selectedMaterialIndex]; }
	BlockMap BlockPositionMap(Scene* scene);
	BlockPreset GetBlockPresetAtPosition(glm::ivec3 position);

private:
	void LoadBrushes(FileManager* fileManager);
	void SelectNextBlock();
	void SelectPreviousBlock();
	void SelectNextColorIndex();
	void SelectPreviousColorIndex();
	void SelectNextCollider();
	void SelectPreviousCollider();
	void CreatePreset();
	void SelectNextPreset();
	void SelectPreviousPreset();
	void SelectNextBrush();
	void SelectPreviousBrush();
	void SelectNextPlacementMode();
	void SelectPreviousPlacementMode();
	void SelectNextMaterial();
	void SelectPreviousMaterial();
	std::string PlacementModeText() const;

	enum class PlacementMode
	{
		Detail = 0,
		Brush = 1,
		Size = 2,
	};

	enum class AxisPlane
	{
		XY,
		XZ,
		YZ,
	};

	std::vector<std::string> blockNames;
	std::vector<std::string> materialNames;
	int selectedBlockIndex;
	int selectedColorIndex;
	int selectedMaterialIndex;
	int selectedBlockColorCount;
	int selectedColliderIndex;
	bool selectedBlockPreviewDirty;
	std::vector<BlockPreset> blockPresets;
	int selectedPreset;
	char newPresetName[32] = { 0 };
	PlacementMode placementMode;
	std::vector<Brush> brushes;
	int selectedBrushIndex;
	AxisPlane brushAxisPlane;
};