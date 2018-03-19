#pragma once
#include <unordered_map>
#include <map>
#include "Shader.h"
#include "Model.h"
#include "Material.h"
#include "Window.h"
#include "FileManager.h"

class ApplicationContext;

struct RenderObject
{
	int id;
	glm::mat4 modelMatrix;
	Mesh* mesh;
	Material* material;
	Shader* shader;
	bool transparent;
};

class Renderer
{
public:
	Renderer(FileManager* fileManager);
	void RenderScene(ApplicationContext* context);
	void DrawLine(ApplicationContext* context, glm::vec3 start, glm::vec3 end);
	Model* GetModel(std::string modelName) { return models[modelName]; }
	int ModelCount() const;
	std::unordered_map<std::string, Model*> Models() const { return models; }
	unsigned int ModelPreviewTextureID() const { return this->modelPreviewTextureID; }
	unsigned int IconTextureID() const { return this->iconTextureID; }
	void RenderModelPreview(std::string modelName, int meshColorIndex, std::string colliderModelName);
	int ModelUVIndexCount(std::string modelName) const;
	int AddRenderObject(std::string meshName, int meshColorIndex, std::string materialName, glm::mat4 modelMatrix);
	void RemoveRenderObject(int id);
	void UpdateRenderObjectModelMatrix(int id, glm::mat4 modelMatrix);
	void ClearScene();

private:
	void LoadShaders();
	void LoadModels(FileManager* fileManager);
	void LoadMaterials();
	void LoadTextures(FileManager* fileManager);
	unsigned int loadTexture(char const * path);

	unsigned int modelPreviewFBO;
	unsigned int modelPreviewTextureID;
	unsigned int iconTextureID;
	const int modelPreviewTextureSize = 256;
	void SetUpModelPreview();

	std::unordered_map<std::string, Shader*> shaders;
	std::unordered_map<std::string, Model*> models;
	std::unordered_map<std::string, std::unique_ptr<Material>> materials;
	std::unordered_map<std::string, unsigned int> textureIDs;

	int renderObjectCounter;
	std::map<int, RenderObject> renderObjects;
};