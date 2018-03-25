#include "Material.h"

Material::Material(tinyxml2::XMLElement * materialNode)
{
	auto shaderNode = materialNode->FirstChildElement("Shader");
	if (shaderNode != nullptr)
	{
		this->shaderName = shaderNode->GetText();
	}

	auto textureNode = materialNode->FirstChildElement("Texture");
	if (textureNode != nullptr)
	{
		this->textureName = textureNode->GetText();
	}

	auto colorNode = materialNode->FirstChildElement("Color");
	if (colorNode != nullptr)
	{
		int r = colorNode->FloatAttribute("r", 1.0f);
		int g = colorNode->FloatAttribute("g", 1.0f);
		int b = colorNode->FloatAttribute("b", 1.0f);

		this->color = glm::vec3(r, g, b);
	}

	auto opacityNode = materialNode->FirstChildElement("Opacity");
	if (opacityNode != nullptr)
	{
		this->opacity = std::stof(opacityNode->GetText());
	}

	auto wireframeNode = materialNode->FirstChildElement("Wireframe");
	if (wireframeNode != nullptr)
	{
		this->wireframe = wireframeNode->BoolText(false);
	}
}
