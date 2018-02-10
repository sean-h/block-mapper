#pragma once
#include <vector>
#include "FileManager.h"

class BlockManager
{
public:
	BlockManager(FileManager* fileManager);
	std::string SelectedBlockName() const { return blockNames[selectedBlockIndex]; }
	void SelectNextBlock() { selectedBlockIndex = (selectedBlockIndex + 1) % blockNames.size(); }
	void SelectPreviousBlock() { selectedBlockIndex = (selectedBlockIndex - 1) % blockNames.size(); }

private:
	std::vector<std::string> blockNames;
	int selectedBlockIndex;
};