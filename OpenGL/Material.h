#pragma once
#include <string>
#include "glm\glm.hpp"

class Material
{
public:
	Material(std::string shaderName) : shaderName(shaderName) {}

	std::string ShaderName() const { return this->shaderName; }

	glm::vec3 Color() const { return this->color; }
	void Color(glm::vec3 color) { this->color = color; }

	float Opacity() const { return this->opacity; }
	void Opacity(float opacity) { this->opacity = opacity; }

	bool IsTransparent() const { return opacity < 1.0f; }

	void Wireframe(bool wireframe) { this->wireframe = wireframe; }
	bool Wireframe() const { return this->wireframe; }

	std::string TextureName() const { return this->textureName; }
	void TextureName(std::string name) { this->textureName = name; }

private:
	std::string shaderName;
	glm::vec3 color;
	float shininess;
	float opacity;
	bool wireframe;
	std::string textureName;
};