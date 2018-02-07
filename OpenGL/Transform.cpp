#include "Transform.h"

glm::mat4 Transform::Model()
{
	glm::mat4 model;
	model = glm::translate(model, position);
	return model;
}

glm::vec3 Transform::Forward() const
{
	glm::vec3 forward;
	forward.x = cos(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	forward.y = sin(glm::radians(rotation.x));
	forward.z = sin(glm::radians(rotation.y)) * cos(glm::radians(rotation.x));
	forward = glm::normalize(forward);
	return forward;
}

glm::vec3 Transform::Right() const
{
	return glm::normalize(glm::cross(Forward(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 Transform::Up() const
{
	return glm::normalize(glm::cross(Right(), Forward()));
}
