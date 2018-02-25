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
		this->textureFolderPath = this->applicationFolderPath + "/textures";

		for (auto &file : std::experimental::filesystem::directory_iterator(this->blockFolderPath))
		{
			this->blockPaths.push_back(file.path());
		}

		for (auto &file : std::experimental::filesystem::directory_iterator(this->textureFolderPath))
		{
			this->texturePaths.push_back(file.path());
		}

		this->exportFolderPath = this->applicationFolderPath + "/" + "Export";
		if (!std::experimental::filesystem::exists(this->exportFolderPath))
		{
			std::experimental::filesystem::create_directory(this->exportFolderPath);
		}

		this->saveFolderPath = this->applicationFolderPath + "/" + "Scenes";
		if (!std::experimental::filesystem::exists(this->saveFolderPath))
		{
			std::experimental::filesystem::create_directory(this->saveFolderPath);
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

std::unordered_map<std::string, std::string> FileManager::TexturePaths() const
{
	std::unordered_map<std::string, std::string> textures;

	for (auto &texturePath : texturePaths)
	{
		textures[texturePath.filename().stem().string()] = texturePath.string();
	}

	return textures;
}

std::vector<std::string> FileManager::SavedSceneFilenames() const
{
	std::vector<std::string> sceneFilenames;

	for (auto &file : std::experimental::filesystem::directory_iterator(this->saveFolderPath))
	{
		sceneFilenames.push_back(file.path().filename().string());
	}

	return sceneFilenames;
}
