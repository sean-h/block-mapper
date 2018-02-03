#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

class Transform
{
public:
	glm::vec3 Position() const { return position; }
	void Position(glm::vec3 position) { this->position = position; }

	glm::vec3 Rotatation() const { return rotatation; }
	glm::vec3 Scale() const { return scale; }
	glm::mat4 Model();

private:
	glm::vec3 position;
	glm::vec3 rotatation;
	glm::vec3 scale;
};