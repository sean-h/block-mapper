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
	this->LoadTextures(fileManager);

	this->SetUpModelPreview();
}

void Renderer::RenderScene(ApplicationContext* context)
{
	Scene* scene = context->ApplicationScene();
	Window* window = context->ApplicationWindow();
	Camera* camera = scene->ActiveCamera();

	glViewport(0, 0, window->Width(), window->Height());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Shader* defaultShader = this->shaders["CameraLit"];
	defaultShader->use();

	glm::mat4 view = camera->ViewMatrix();
	glm::mat4 projection = camera->ProjectionMatrix((float)window->Width(), (float)window->Height());

	for (auto &shader : this->shaders)
	{
		shader.second->use();
		shader.second->setMat4("view", glm::value_ptr(view));
		shader.second->setMat4("projection", glm::value_ptr(projection));

		if (this->textureIDs.size() > 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, this->textureIDs["Palette"]);
		}
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

		glm::mat4 modelMatrix = e->ObjectTransform()->Model();
		
		Shader* shader = this->shaders[material->ShaderName()];
		shader->use();
		shader->setMat4("model", glm::value_ptr(modelMatrix));
		shader->setVec3("objectColor", material->Color());

		auto model = models.find(e->MeshName());
		if (model != models.end())
		{
			model->second->Draw(*shader, e->MeshColorIndex());
		}
	}

	// Draw transparent entities
	Shader* transparentShader = this->shaders["Transparent"];
	transparentShader->use();
	for (auto &e : transparentEntities)
	{
		Material* material = materials[e->MaterialName()].get();
		glm::mat4 modelMatrix = e->ObjectTransform()->Model();
		transparentShader->setMat4("model", glm::value_ptr(modelMatrix));
		transparentShader->setVec3("objectColor", material->Color());
		transparentShader->setFloat("opacity", material->Opacity());

		auto model = models.find(e->MeshName());
		if (model != models.end())
		{
			model->second->Draw(*transparentShader, e->MeshColorIndex());
		}
	}
}

void Renderer::DrawLine(ApplicationContext* context, glm::vec3 start, glm::vec3 end)
{
	Window* window = context->ApplicationWindow();
	Camera* camera = context->ApplicationScene()->ActiveCamera();

	glm::mat4 view = camera->ViewMatrix();
	glm::mat4 projection = camera->ProjectionMatrix((float)window->Width(), (float)window->Height());
	glm::mat4 model;

	Transform transform;
	transform.Position(start);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 look = glm::normalize(end - start);
	if (look == up)
	{
		up = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	transform.LookAt(look, up);
	transform.Scale(glm::vec3(1.0f, 1.0f, glm::distance(start, end)));

	model = transform.Model();

	Shader* shader = this->shaders["Unlit"];
	shader->use();
	shader->setMat4("view", glm::value_ptr(view));
	shader->setMat4("projection", glm::value_ptr(projection));
	shader->setMat4("model", glm::value_ptr(model));
	shader->setVec3("objectColor", glm::vec3(1.0f, 0.0f, 0.0f));

	models["Line"]->Draw(*shader);
}

int Renderer::ModelCount() const
{
	int modelCount = 0;
	for (auto& model : models)
	{
		modelCount += model.second->UVIndexCount();
	}

	return modelCount;
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
	this->models["Line"] = new Model("Line.fbx");

	for (auto &block : fileManager->BlockPaths())
	{
		this->models[block.first] = new Model(block.second);

		if (this->models[block.first]->ActiveMesh() == nullptr)
		{
			std::cout << "Could not load model: " << block.first << std::endl;
			models.erase(block.first);
		}
	}
}

void Renderer::LoadMaterials()
{
	std::unique_ptr<Material> solidMaterial(new Material("CameraLit"));
	solidMaterial->Color(glm::vec3(1.0f, 1.0f, 1.0f));
	solidMaterial->Opacity(1.0f);

	std::unique_ptr<Material> selectedMaterial(new Material("CameraLit"));
	selectedMaterial->Color(glm::vec3(0.2f, 0.8f, 0.2f));
	selectedMaterial->Opacity(1.0f);

	std::unique_ptr<Material> hoverMaterial(new Material("Transparent"));
	hoverMaterial->Color(glm::vec3(0.2f, 0.2f, 0.6f));
	hoverMaterial->Opacity(0.5f);

	std::unique_ptr<Material> gridMaterial(new Material("Grid"));
	gridMaterial->Color(glm::vec3(0.0f, 0.0f, 0.0f));
	gridMaterial->Opacity(1.0f);

	this->materials["Solid"] = std::move(solidMaterial);
	this->materials["Selected"] = std::move(selectedMaterial);
	this->materials["Hover"] = std::move(hoverMaterial);
	this->materials["Grid"] = std::move(gridMaterial);
}

void Renderer::LoadTextures(FileManager * fileManager)
{
	for (auto& texture : fileManager->TexturePaths())
	{
		this->textureIDs[texture.first] = this->loadTexture(texture.second.c_str());
	}
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

void Renderer::SetUpModelPreview()
{
	glGenFramebuffers(1, &this->modelPreviewFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->modelPreviewFBO);

	glGenTextures(1, &this->modelPreviewTextureID);
	glBindTexture(GL_TEXTURE_2D, this->modelPreviewTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, modelPreviewTextureSize, modelPreviewTextureSize, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->modelPreviewTextureID, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::RenderModelPreview(std::string modelName)
{
	glViewport(0, 0, modelPreviewTextureSize, modelPreviewTextureSize);
	glBindFramebuffer(GL_FRAMEBUFFER, this->modelPreviewFBO);

	if (models.find(modelName) != models.end())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Shader* defaultShader = this->shaders["CameraLit"];
		defaultShader->use();

		glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 3.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		defaultShader->setMat4("view", glm::value_ptr(view));
		defaultShader->setMat4("projection", glm::value_ptr(projection));
		defaultShader->setMat4("model", glm::value_ptr(model));
		defaultShader->setVec3("objectColor", glm::vec3(0.2f, 0.2f, 0.8f));
		defaultShader->setVec3("cameraPosition", glm::vec3(0.0f, 0.0f, 0.0f));
		models[modelName]->Draw(*defaultShader);
	}
	else
	{
		// Model could not be found
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int Renderer::ModelUVIndexCount(std::string modelName) const
{
	auto model = models.find(modelName);
	if (model != models.end())
	{
		return model->second->UVIndexCount();
	}

	return 0;
}
