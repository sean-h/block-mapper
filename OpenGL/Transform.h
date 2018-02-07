#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"

class Transform
{
public:
	glm::vec3 Position() const { return position; }
	void Position(glm::vec3 position) { this->position = position; }

	glm::vec3 Rotation() const { return rotation; }
	void Rotation(glm::vec3 rotation) { this->rotation = rotation; }

	glm::vec3 Scale() const { return scale; }
	glm::mat4 Model();
	glm::vec3 Forward() const;
	glm::vec3 Right() const;
	glm::vec3 Up() const;

private:
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};