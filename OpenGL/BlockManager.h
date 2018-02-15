#pragma once
#include <vector>
#include "FileManager.h"

class ApplicationContext;

class BlockManager
{
public:
	BlockManager(FileManager* fileManager);
	void DrawGUI(ApplicationContext* context);
	std::string SelectedBlockName() const { return blockNames[selectedBlockIndex]; }
	void SelectNextBlock();
	void SelectPreviousBlock();

private:
	std::vector<std::string> blockNames;
	int selectedBlockIndex;
	bool selectedBlockPreviewDirty;
};