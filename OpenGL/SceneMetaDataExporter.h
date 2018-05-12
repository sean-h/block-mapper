#pragma once
#include <string>

class Scene;

class SceneMetaDataExporter
{
public:
	SceneMetaDataExporter(std::string metaDataExportPath);
	void Export(Scene* scene);

private:
	std::string metaDataExportPath;
};