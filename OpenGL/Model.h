#pragma once
#include <vector>
#include <unordered_map>
#include <memory>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"

class Model
{
public:
	/*  Functions   */
	Model(std::string path)
	{
		loadModel(path);
	}
	void Draw(Shader shader);
	void Draw(Shader shader, int meshColorIndex);
	Mesh* ActiveMesh() const { return activeMesh; }

	Mesh* GetMesh(int meshIndex) { return meshes[meshIndex].get(); }
	int UVIndexCount() { return meshes.size(); }

private:
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene, int uvChannelIndex);
	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma);

	std::unordered_map<int, std::unique_ptr<Mesh>> meshes;
	Mesh* activeMesh;
	std::string directory;
};