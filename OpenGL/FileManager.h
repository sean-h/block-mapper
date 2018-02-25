#pragma once
#include <string>
#include <vector>
#include <filesystem>
#include <unordered_map>

class FileManager
{
public:
	FileManager();
	std::vector<std::string> BlockNames() const;
	std::unordered_map<std::string, std::string> BlockPaths() const;
	std::unordered_map<std::string, std::string> TexturePaths() const;
	std::string ExportFilePath() const { return exportFolderPath + "/"; }
	std::string SaveFilePath() const { return saveFolderPath + "/"; }
	std::vector<std::string> SavedSceneFilenames() const;

private:
	const std::string applicationFolderName = "BlockEditor";
	std::string applicationFolderPath;
	std::string blockFolderPath;
	std::string textureFolderPath;
	std::vector<std::experimental::filesystem::path> blockPaths;
	std::vector<std::experimental::filesystem::path> texturePaths;
	std::string exportFolderPath;
	std::string saveFolderPath;
};