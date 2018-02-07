#include "OrbitController.h"
#include "Entity.h"
#include "ApplicationContext.h"
#include <iostream>
#include "glm\glm.hpp"
#include "glm/gtx/quaternion.hpp"

OrbitController::OrbitController()
{
	this->orbitDistance = 10.0f;
	this->orbitPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	this->yAxisRotation = 270.0f;
}

void OrbitController::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();
	if (input->GetKey(Input::Keys::MOUSE_3))
	{
		yAxisRotation += input->MouseXDelta() * 0.02f;
		float x = this->orbitDistance * glm::cos(glm::radians(yAxisRotation));
		float z = this->orbitDistance * glm::sin(glm::radians(yAxisRotation));
		glm::vec3 newPos = glm::vec3(x, 0.0f, z);

		Transform* transform = this->Owner()->ObjectTransform();
		transform->Position(newPos);
		transform->Rotation(glm::vec3(0.0f, 180.0f + yAxisRotation, 0.0f));
	}
}
