#pragma once
#include <unordered_map>
#include "Scene.h"
#include "Shader.h"
#include "Model.h"
#include "Window.h"

class Renderer
{
public:
	Renderer();
	void RenderScene(Scene &scene, Window &window, Camera &camera);

private:
	void LoadShaders();
	void LoadModels();
	unsigned int loadTexture(char const * path);

	std::unordered_map<std::string, Shader*> shaders;
	std::unordered_map<std::string, Model*> models;
	std::unordered_map<std::string, unsigned int> textureIDs;
};