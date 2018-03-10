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

class BlockManager
{
public:
	BlockManager(FileManager* fileManager);
	void Update(ApplicationContext* context);
	void DrawGUI(ApplicationContext* context);
	std::string SelectedBlockName() const { return blockNames[selectedBlockIndex]; }
	int SelectedColorIndex() const { return selectedColorIndex; }
	void SelectNextBlock();
	void SelectPreviousBlock();
	void SelectNextColorIndex();
	void SelectPreviousColorIndex();
	void SelectNextCollider();
	void SelectPreviousCollider();
	std::string SelectedColliderName() const { return blockNames[selectedColliderIndex]; }
	BlockMap BlockPositionMap(Scene* scene);

private:
	std::vector<std::string> blockNames;
	int selectedBlockIndex;
	int selectedColorIndex;
	int selectedBlockColorCount;
	int selectedColliderIndex;
	bool selectedBlockPreviewDirty;
};