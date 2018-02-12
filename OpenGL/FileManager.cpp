#include "FileManager.h"
#include <shlobj.h>
#include <iostream>

FileManager::FileManager()
{
	CHAR my_documents[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, my_documents);
	if (result == S_OK)
	{
		this->applicationFolderPath = std::string(my_documents) + "/" + applicationFolderName;
		this->blockFolderPath = this->applicationFolderPath + "/blocks";

		for (auto &file : std::experimental::filesystem::directory_iterator(this->blockFolderPath))
		{
			this->blockPaths.push_back(file.path());
		}

		this->exportFileName = "Export.dae";
		this->exportFolderPath = this->applicationFolderPath + "/" + "Export";

		if (!std::experimental::filesystem::exists(this->exportFolderPath))
		{
			std::experimental::filesystem::create_directory(this->exportFolderPath);
		}
	}
}

std::vector<std::string> FileManager::BlockNames() const
{
	std::vector<std::string> blockNames;

	for (auto &blockPath : blockPaths)
	{
		blockNames.push_back(blockPath.filename().stem().string());
	}

	return blockNames;
}

std::unordered_map<std::string, std::string> FileManager::BlockPaths() const
{
	std::unordered_map<std::string, std::string> blocks;

	for (auto &blockPath : blockPaths)
	{
		blocks[blockPath.filename().stem().string()] = blockPath.string();
	}

	return blocks;
}
