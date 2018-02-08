#include "Transform.h"

glm::mat4 Transform::Model()
{
	glm::mat4 model;
	model = glm::translate(model, position);
	return model;
}

glm::vec3 Transform::Forward() const
{
	return this->rotation * glm::vec3(0.0f, 0.0f, 1.0f);
}

glm::vec3 Transform::Right() const
{
	return glm::normalize(glm::cross(Forward(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 Transform::Up() const
{
	return glm::normalize(glm::cross(Right(), Forward()));
}

void Transform::Rotate(glm::vec3 axis, float angle)
{
	rotation = glm::rotate(rotation, glm::radians(angle), axis);
}

void Transform::LookAt(glm::vec3 direction, glm::vec3 up)
{
	glm::mat3x3 mat;

	mat[2] = direction;
	mat[0] = glm::normalize(glm::cross(up, mat[2]));
	mat[1] = glm::cross(mat[2], mat[0]);

	this->rotation = glm::quat_cast(mat);
}

void Transform::RotateAroundPoint(glm::vec3 point, glm::vec3 axis, float degrees)
{
	glm::vec3 p = position - point;
	glm::quat q(glm::vec3(axis.x * glm::radians(degrees),
		axis.y * glm::radians(degrees),
		axis.z * glm::radians(degrees)));
	p = q * p;
	position = p + point;
}
