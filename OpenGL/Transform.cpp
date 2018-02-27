#include "Transform.h"

Transform::Transform()
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->scale = glm::vec3(1.0f, 1.0f, 1.0f);
	this->UpdateModel();
}

void Transform::Position(glm::vec3 position)
{
	this->position = position;
	this->UpdateModel();
}

void Transform::Rotation(glm::vec3 rotation)
{
	this->rotation = glm::quat(glm::vec3(glm::radians(rotation.x), glm::radians(rotation.y), glm::radians(rotation.z)));
	this->UpdateModel();
}

void Transform::Rotation(glm::quat rotation)
{
	this->rotation = rotation;
	this->UpdateModel();
}

void Transform::Scale(glm::vec3 scale)
{
	this->scale = scale;
	this->UpdateModel();
}

glm::mat4 Transform::Model()
{
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
	rotation = glm::rotate(glm::quat(), glm::radians(angle), axis) * rotation;
	this->UpdateModel();
}

void Transform::LookAt(glm::vec3 direction, glm::vec3 up)
{
	glm::mat3x3 mat;

	mat[2] = direction;
	mat[0] = glm::normalize(glm::cross(up, mat[2]));
	mat[1] = glm::cross(mat[2], mat[0]);

	this->rotation = glm::quat_cast(mat);
	this->UpdateModel();
}

void Transform::RotateAroundPoint(glm::vec3 point, glm::vec3 axis, float degrees)
{
	glm::vec3 p = position - point;
	glm::quat q(glm::vec3(axis.x * glm::radians(degrees),
		axis.y * glm::radians(degrees),
		axis.z * glm::radians(degrees)));
	p = q * p;
	position = p + point;
	this->UpdateModel();
}

void Transform::Translate(glm::vec3 translation)
{
	this->position += translation;
	this->UpdateModel();
}

void Transform::UpdateModel()
{
	glm::mat4 m;
	m = glm::translate(m, position);
	m = m * glm::mat4_cast(rotation);
	m = glm::scale(m, scale);
	this->model = m;
}
