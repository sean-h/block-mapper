#include "OrbitController.h"
#include "Entity.h"
#include "ApplicationContext.h"
#include <iostream>
#include "glm\glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.inl"

OrbitController::OrbitController()
{
	this->orbitPoint = glm::vec3(0.0f, 0.0f, 0.0f);
}

void OrbitController::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();
	if (input->GetKey(Input::Keys::MOUSE_3))
	{
		Transform* transform = this->Owner()->ObjectTransform();

		float rightRotation = input->MouseYDelta() * 0.1f;
		glm::vec3 forward = transform->Forward();

		if (rightRotation < 0 && glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), forward) > -clampRotation ||
			rightRotation > 0 && glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), forward) < clampRotation)
		{
			transform->RotateAroundPoint(orbitPoint, transform->Right(), rightRotation);
		}
		transform->RotateAroundPoint(orbitPoint, transform->Up(), -input->MouseXDelta() * 0.1f);

		glm::vec3 lookDirection = glm::normalize(this->orbitPoint - transform->Position());
		transform->LookAt(lookDirection, glm::vec3(0.0f, 1.0f, 0.0f));
	}
}
