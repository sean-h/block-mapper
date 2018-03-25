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
		this->blockFolderPath = this->applicationFolderPath + "/Blocks";
		CreateDirectoryAtPath(blockFolderPath);

		this->textureFolderPath = this->applicationFolderPath + "/Textures";
		CreateDirectoryAtPath(textureFolderPath);

		this->materialFolderPath = this->applicationFolderPath + "/Materials";
		CreateDirectoryAtPath(materialFolderPath);

		for (auto &file : std::experimental::filesystem::directory_iterator(this->blockFolderPath))
		{
			this->blockPaths.push_back(file.path());
		}

		for (auto &file : std::experimental::filesystem::directory_iterator(this->textureFolderPath))
		{
			this->texturePaths.push_back(file.path());
		}

		for (auto &file : std::experimental::filesystem::directory_iterator(this->materialFolderPath))
		{
			this->materialPaths.push_back(file.path());
		}

		this->exportFolderPath = this->applicationFolderPath + "/" + "Export";
		CreateDirectoryAtPath(exportFolderPath);

		this->saveFolderPath = this->applicationFolderPath + "/" + "Scenes";
		CreateDirectoryAtPath(saveFolderPath);
	}
}

std::vector<std::string> FileManager::BlockNames() const
{
	std::vector<std::string> blockNames;

	// Add application blocks
	blockNames.push_back("Cube");

	for (auto &blockPath : blockPaths)
	{
		blockNames.push_back(blockPath.filename().stem().string());
	}

	return blockNames;
}

std::vector<std::string> FileManager::MaterialNames() const
{
	std::vector<std::string> materialNames;

	for (auto& materialPath : materialPaths)
	{
		materialNames.push_back(materialPath.filename().stem().string());
	}

	return materialNames;
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

std::unordered_map<std::string, std::string> FileManager::MaterialPaths() const
{
	std::unordered_map<std::string, std::string> materials;

	for (auto& materialPath : materialPaths)
	{
		materials[materialPath.filename().stem().string()] = materialPath.string();
	}

	return materials;
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

bool FileManager::FileExists(std::string filePath) const
{
	return std::experimental::filesystem::exists(filePath);
}

void FileManager::CreateDirectoryAtPath(std::string directory)
{
	if (!std::experimental::filesystem::exists(directory))
	{
		std::experimental::filesystem::create_directory(directory);
	}
}
