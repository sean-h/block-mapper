#pragma once
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtc\quaternion.hpp"

class Transform
{
public:
	glm::vec3 Position() const { return position; }
	void Position(glm::vec3 position) { this->position = position; }

	glm::vec3 Rotation() const
	{ 
		glm::vec3 r = glm::eulerAngles(rotation);
		return glm::vec3(glm::degrees(r.x), glm::degrees(r.y), glm::degrees(r.z));
	}
	glm::vec3 RotationRadians() const { return glm::eulerAngles(rotation); }
	void Rotation(glm::vec3 rotation) { this->rotation = glm::quat(rotation); }
	void Rotation(glm::quat rotation) { this->rotation = rotation; }
	glm::quat RotationQuaternion() const { return rotation; }

	glm::vec3 Scale() const { return scale; }
	glm::mat4 Model();
	glm::vec3 Forward() const;
	glm::vec3 Right() const;
	glm::vec3 Up() const;

	void Rotate(glm::vec3 axis, float angle);
	void LookAt(glm::vec3 direction, glm::vec3 up);
	void RotateAroundPoint(glm::vec3 point, glm::vec3 axis, float degrees);
	void Translate(glm::vec3 translation) { this->position += translation; }

private:
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;
};