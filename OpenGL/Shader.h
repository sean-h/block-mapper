#pragma once
#include <Glad\glad.h>
#include <string>
#include "glm\glm.hpp"

class Shader
{
public:
	unsigned int ID;

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void use();

	void setBool(const std::string &name, bool value) const;

	void setInt(const std::string &name, int value) const;

	void setFloat(const std::string &name, float value) const;
	void setFloat(GLint locationID, float value) const;

	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec3(const std::string &name, glm::vec3 vec3) const;
	void setVec3(GLint locationID, glm::vec3 vec3) const;

	void setMat4(const std::string &name, float* mat) const;
	void setMat4(GLint locationID, float* mat) const;

	GLint UniformLocation(const std::string &name) const;
};