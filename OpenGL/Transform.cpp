#include "Transform.h"

glm::mat4 Transform::Model()
{
	glm::mat4 model;
	model = glm::translate(model, position);
	return model;
}
