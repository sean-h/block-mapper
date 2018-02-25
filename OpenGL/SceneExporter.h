#pragma once
#include "ApplicationContext.h"
#include <assimp\Exporter.hpp>
#include <assimp\scene.h>
#include <memory>
#include "Mesh.h"

class SceneExporter
{
public:
	void Export(ApplicationContext* context);

private:
	std::unique_ptr<aiScene> exportScene;
	std::unordered_map<std::string, std::unordered_map<int, int>> meshIndexes;

	void LoadMeshData(aiScene* scene, Mesh* mesh, int meshIndex);
};