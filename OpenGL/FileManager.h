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

private:
	const std::string applicationFolderName = "BlockEditor";
	std::string applicationFolderPath;
	std::string blockFolderPath;
	std::vector<std::experimental::filesystem::path> blockPaths;
};