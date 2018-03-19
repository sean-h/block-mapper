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
	renderObjectCounter = 0;
}

void Renderer::RenderScene(ApplicationContext* context)
{
	Scene* scene = context->ApplicationScene();
	Window* window = context->ApplicationWindow();
	Camera* camera = scene->ActiveCamera();

	glViewport(0, 0, window->Width(), window->Height());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureIDs["Palette"]);

	glm::mat4 view = camera->ViewMatrix();
	glm::mat4 projection = camera->ProjectionMatrix((float)window->Width(), (float)window->Height());

	std::map<Shader*, std::vector<RenderObject>> shaderSortedRenderObjects;
	for (auto& renderObjectPair : renderObjects)
	{
		shaderSortedRenderObjects[renderObjectPair.second.shader].push_back(renderObjectPair.second);
	}

	for (auto& shaderSortedRenderObject : shaderSortedRenderObjects)
	{
		Shader* shader = shaderSortedRenderObject.first;
		shader->use();

		shader->setVec3(shader->cameraPositionID, camera->Owner()->ObjectTransform()->Position());
		shader->setMat4(shader->viewID, glm::value_ptr(view));
		shader->setMat4(shader->projectionID, glm::value_ptr(projection));

		for (auto& renderObject : shaderSortedRenderObject.second)
		{
			shader->setMat4(shader->modelID, glm::value_ptr(renderObject.modelMatrix));
			shader->setVec3(shader->objectColorID, renderObject.material->Color());
			shader->setFloat(shader->opacityID, renderObject.material->Opacity());
			shader->setFloat(shader->inverseColorMultiplierID, 0.0f);

			renderObject.mesh->Draw(*shader);

			if (renderObject.material->Wireframe())
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				renderObject.shader->setFloat(shader->inverseColorMultiplierID, 1.0f);
				renderObject.mesh->Draw(*shader);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			}
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
	shaders["CameraLit"] = new Shader("VertexShader.glsl", "CameraLitFragmentShader.glsl");
	Shader* cameraLit = shaders["CameraLit"];
	cameraLit->use();
	cameraLit->cameraPositionID = cameraLit->UniformLocation("cameraPosition");
	cameraLit->viewID = cameraLit->UniformLocation("view");
	cameraLit->projectionID = cameraLit->UniformLocation("projection");
	cameraLit->modelID = cameraLit->UniformLocation("model");
	cameraLit->objectColorID = cameraLit->UniformLocation("objectColor");
	cameraLit->opacityID = cameraLit->UniformLocation("opacity");
	cameraLit->inverseColorMultiplierID = cameraLit->UniformLocation("inverseColorMultiplier");


	shaders["Grid"] = new Shader("VertexShader.glsl", "GridFragmentShader.glsl");
	Shader* grid = shaders["Grid"];
	grid->use();
	grid->cameraPositionID = grid->UniformLocation("cameraPosition");
	grid->viewID = grid->UniformLocation("view");
	grid->projectionID = grid->UniformLocation("projection");
	grid->modelID = grid->UniformLocation("model");
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
	solidMaterial->Wireframe(false);

	std::unique_ptr<Material> selectedMaterial(new Material("CameraLit"));
	selectedMaterial->Color(glm::vec3(0.2f, 0.8f, 0.2f));
	selectedMaterial->Opacity(1.0f);
	selectedMaterial->Wireframe(true);

	std::unique_ptr<Material> hoverMaterial(new Material("CameraLit"));
	hoverMaterial->Color(glm::vec3(1.0f, 1.0f, 1.0f));
	hoverMaterial->Opacity(1.0f);
	hoverMaterial->Wireframe(true);

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

	iconTextureID = this->loadTexture("Icons.png");
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

void Renderer::RenderModelPreview(std::string modelName, int meshColorIndex, std::string colliderModelName)
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

		defaultShader->setMat4(defaultShader->viewID, glm::value_ptr(view));
		defaultShader->setMat4(defaultShader->projectionID, glm::value_ptr(projection));
		defaultShader->setMat4(defaultShader->modelID, glm::value_ptr(model));
		defaultShader->setVec3(defaultShader->objectColorID, glm::vec3(1.0f, 1.0f, 1.0f));
		defaultShader->setVec3(defaultShader->cameraPositionID, glm::vec3(0.0f, 0.0f, 0.0f));
		defaultShader->setFloat(defaultShader->inverseColorMultiplierID, 0.0f);
		models[modelName]->Draw(*defaultShader, meshColorIndex);

		if (colliderModelName != "")
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			defaultShader->setFloat(defaultShader->inverseColorMultiplierID, 1.0f);
			models[colliderModelName]->Draw(*defaultShader);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
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

int Renderer::AddRenderObject(std::string meshName, int meshColorIndex, std::string materialName, glm::mat4 modelMatrix)
{
	RenderObject renderObject;
	renderObject.material = this->materials[materialName].get();
	renderObject.mesh = this->models[meshName]->GetMesh(meshColorIndex);
	renderObject.shader = this->shaders[renderObject.material->ShaderName()];
	renderObject.transparent = renderObject.material->IsTransparent();
	renderObject.modelMatrix = modelMatrix;
	renderObject.id = ++renderObjectCounter;

	this->renderObjects[renderObject.id] = renderObject;

	return renderObject.id;
}

void Renderer::RemoveRenderObject(int id)
{
	if (id <= 0)
	{
		return;
	}

	renderObjects.erase(id);
}

void Renderer::UpdateRenderObjectModelMatrix(int id, glm::mat4 modelMatrix)
{
	if (id <= 0)
	{
		return;
	}

	this->renderObjects[id].modelMatrix = modelMatrix;
}

void Renderer::ClearScene()
{
	renderObjects.clear();
}
