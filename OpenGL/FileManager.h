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
	std::vector<std::string> MaterialNames() const;
	std::unordered_map<std::string, std::string> BlockPaths() const;
	std::unordered_map<std::string, std::string> TexturePaths() const;
	std::unordered_map<std::string, std::string> MaterialPaths() const;
	std::unordered_map<std::string, std::string> BrushPaths() const;
	std::string ExportFilePath() const { return exportFolderPath + "/"; }
	std::string SaveFilePath() const { return saveFolderPath + "/"; }
	std::vector<std::string> SavedSceneFilenames() const;
	bool FileExists(std::string filePath) const;

private:
	void CreateDirectoryAtPath(std::string directory);

	const std::string applicationFolderName = "BlockEditor";
	std::string applicationFolderPath;
	std::string blockFolderPath;
	std::string textureFolderPath;
	std::string materialFolderPath;
	std::string brushFolderPath;
	std::vector<std::experimental::filesystem::path> blockPaths;
	std::vector<std::experimental::filesystem::path> texturePaths;
	std::vector<std::experimental::filesystem::path> materialPaths;
	std::vector<std::experimental::filesystem::path> brushPaths;
	std::string exportFolderPath;
	std::string saveFolderPath;
};