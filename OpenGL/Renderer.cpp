#include "Camera.h"
#include "ApplicationContext.h"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\type_ptr.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

Renderer::Renderer()
{
	this->LoadShaders();
	this->LoadModels();
}

void Renderer::RenderScene(ApplicationContext* context)
{
	Scene* scene = context->ApplicationScene();
	Window* window = context->ApplicationWindow();
	Camera* camera = scene->ActiveCamera();

	Shader* defaultShader = this->shaders["Default"];
	defaultShader->use();

	glm::mat4 view = camera->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(defaultShader->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)window->Width() / (float)window->Height(), 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(defaultShader->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glm::mat4 lightModel;
	lightModel = glm::rotate(lightModel, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
	lightModel = glm::translate(lightModel, glm::vec3(4.0f, 0.0f, 0.0f));
	lightModel = glm::scale(lightModel, glm::vec3(0.2f, 0.2f, 0.2f));
	glm::vec3 lightPosition = lightModel * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	defaultShader->setFloat("material.shininess", 32.0f);
	defaultShader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
	defaultShader->setVec3("light.diffuse", 0.5f, 0.5f, 0.5f); // darken the light a bit to fit the scene
	defaultShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
	defaultShader->setFloat("light.constant", 1.0f);
	defaultShader->setFloat("light.linear", 0.09f);
	defaultShader->setFloat("light.quadratic", 0.032f);
	defaultShader->setVec3("light.position", lightPosition.x, lightPosition.y, lightPosition.z);

	for (auto &e : scene->Entities())
	{
		glm::mat4 model = e->ObjectTransform()->Model();
		defaultShader->setMat4("model", glm::value_ptr(model));
		models["Cube"]->Draw(*defaultShader);
	}

	// Draw light
	Shader* lightShader = this->shaders["Light"];
	lightShader->use();

	lightShader->setMat4("model", glm::value_ptr(lightModel));
	lightShader->setMat4("view", glm::value_ptr(view));
	lightShader->setMat4("projection", glm::value_ptr(projection));
	lightShader->setVec3("objectColor", 1.0f, 1.0f, 1.0f);
	models["Cube"]->Draw(*lightShader);
}

void Renderer::LoadShaders()
{
	shaders["Default"] = new Shader("VertexShader.glsl", "FragmentShader.glsl");
	shaders["Light"] = new Shader("VertexShader.glsl", "UnlitFragmentShader.glsl");

	unsigned int diffuseMap = loadTexture("container2.png");
	unsigned int specularMap = loadTexture("container2_specular.png");
	shaders["Default"]->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);
}

void Renderer::LoadModels()
{
	this->models["Cube"] = new Model("Cube.fbx");
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
