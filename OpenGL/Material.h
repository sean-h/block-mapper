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

private:
	std::string shaderName;
	glm::vec3 color;
	float shininess;
	float opacity;
};