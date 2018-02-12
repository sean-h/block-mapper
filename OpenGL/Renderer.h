#pragma once
#include <unordered_map>
#include "Shader.h"
#include "Model.h"
#include "Material.h"
#include "Window.h"
#include "FileManager.h"

class ApplicationContext;

class Renderer
{
public:
	Renderer(FileManager* fileManager);
	void RenderScene(ApplicationContext* context);
	Model* GetModel(std::string modelName) { return models[modelName]; }
	int ModelCount() const { return models.size(); }
	std::unordered_map<std::string, Model*> Models() const { return models; }

private:
	void LoadShaders();
	void LoadModels(FileManager* fileManager);
	void LoadMaterials();
	unsigned int loadTexture(char const * path);

	std::unordered_map<std::string, Shader*> shaders;
	std::unordered_map<std::string, Model*> models;
	std::unordered_map<std::string, std::unique_ptr<Material>> materials;
	std::unordered_map<std::string, unsigned int> textureIDs;
};