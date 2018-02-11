#include "Camera.h"
#include "ApplicationContext.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

Renderer::Renderer(FileManager* fileManager)
{
	this->LoadShaders();
	this->LoadModels(fileManager);
	this->LoadMaterials();
}

void Renderer::RenderScene(ApplicationContext* context)
{
	Scene* scene = context->ApplicationScene();
	Window* window = context->ApplicationWindow();
	Camera* camera = scene->ActiveCamera();

	Shader* defaultShader = this->shaders["CameraLit"];
	defaultShader->use();

	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window->Width() / (float)window->Height(), 0.1f, 1000.0f);

	for (auto &shader : this->shaders)
	{
		shader.second->use();
		shader.second->setMat4("view", glm::value_ptr(view));
		shader.second->setMat4("projection", glm::value_ptr(projection));
	}

	std::vector<Entity*> transparentEntities;

	defaultShader->use();
	defaultShader->setVec3("cameraPosition", camera->Owner()->ObjectTransform()->Position());
	for (auto &e : scene->Entities())
	{
		if (e->MeshName().length() == 0)
		{
			continue;
		}

		Material* material = materials[e->MaterialName()].get();
		if (material->Opacity() < 1.0f)
		{
			transparentEntities.push_back(e.get());
			continue;
		}

		glm::mat4 model = e->ObjectTransform()->Model();
		
		Shader* shader = this->shaders[material->ShaderName()];
		shader->use();
		shader->setMat4("model", glm::value_ptr(model));
		shader->setVec3("objectColor", material->Color());
		models[e->MeshName()]->Draw(*shader);
	}

	// Draw transparent entities
	Shader* transparentShader = this->shaders["Transparent"];
	transparentShader->use();
	for (auto &e : transparentEntities)
	{
		Material* material = materials[e->MaterialName()].get();
		glm::mat4 model = e->ObjectTransform()->Model();
		transparentShader->setMat4("model", glm::value_ptr(model));
		transparentShader->setVec3("objectColor", material->Color());
		transparentShader->setFloat("opacity", material->Opacity());
		models[e->MeshName()]->Draw(*transparentShader);
	}
}

void Renderer::LoadShaders()
{
	shaders["Default"] = new Shader("VertexShader.glsl", "FragmentShader.glsl");
	shaders["Unlit"] = new Shader("VertexShader.glsl", "UnlitFragmentShader.glsl");
	shaders["CameraLit"] = new Shader("VertexShader.glsl", "CameraLitFragmentShader.glsl");
	shaders["Transparent"] = new Shader("VertexShader.glsl", "TransparentFragmentShader.glsl");
	shaders["Grid"] = new Shader("VertexShader.glsl", "GridFragmentShader.glsl");

	unsigned int diffuseMap = loadTexture("container2.png");
	unsigned int specularMap = loadTexture("container2_specular.png");
	shaders["Default"]->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
}

void Renderer::LoadModels(FileManager* fileManager)
{
	this->models["Cube"] = new Model("Cube.fbx");
	this->models["Plane"] = new Model("Plane.fbx");
	this->models["PlaneBottom"] = new Model("PlaneBottom.fbx");

	for (auto &block : fileManager->BlockPaths())
	{
		this->models[block.first] = new Model(block.second);
	}
}

void Renderer::LoadMaterials()
{
	std::unique_ptr<Material> solidMaterial(new Material("CameraLit"));
	solidMaterial->Color(glm::vec3(0.6f, 0.2f, 0.2f));
	solidMaterial->Opacity(1.0f);

	std::unique_ptr<Material> hoverMaterial(new Material("Transparent"));
	hoverMaterial->Color(glm::vec3(0.2f, 0.2f, 0.6f));
	hoverMaterial->Opacity(0.5f);

	std::unique_ptr<Material> gridMaterial(new Material("Grid"));
	gridMaterial->Color(glm::vec3(0.0f, 0.0f, 0.0f));
	gridMaterial->Opacity(1.0f);

	this->materials["Solid"] = std::move(solidMaterial);
	this->materials["Hover"] = std::move(hoverMaterial);
	this->materials["Grid"] = std::move(gridMaterial);
}

unsigned int Renderer::loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
