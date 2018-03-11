#pragma once
#include "ApplicationContext.h"
#include <assimp\Exporter.hpp>
#include <assimp\scene.h>
#include <memory>
#include "Mesh.h"

struct ExportMeshData
{
	std::vector<aiVector3D> vertexPositions;
	std::vector<aiVector3D> vertexNormals;
	std::vector<aiVector3D> vertexTextureCoords;
	std::vector<aiFace> faces;

	void TransformVertexPositions(glm::mat4 transformMatrix);
};

struct ExportObject
{
	std::string name;
	std::shared_ptr<ExportMeshData> exportMeshData;
	aiMatrix4x4 transformMatrix;
};

class SceneExporter
{
public:
	void Export(ApplicationContext* context);

private:
	std::unique_ptr<aiScene> exportScene;
	std::unordered_map<std::string, std::unordered_map<int, int>> meshIndexes;
	std::unordered_map<std::string, std::shared_ptr<ExportMeshData>> exportMeshes;
	std::vector<std::shared_ptr<ExportMeshData>> exportMeshesList;
	std::vector<ExportObject> exportObjects;

	ExportMeshData GetMeshData(Mesh* mesh);
	void AddMeshDataToScene(aiScene* scene, ExportMeshData* meshData, int meshIndex);
	void CreateExportObject(Entity* entity);
	int GetExportMeshDataID(std::shared_ptr<ExportMeshData> exportMeshData);
};

aiMatrix4x4 TransformMatrix(glm::vec3 position, glm::quat rotation, glm::vec3 scale);