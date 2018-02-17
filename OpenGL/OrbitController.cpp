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

void OrbitController::OnAttachToEntity()
{
	this->Rotate(0.0f, 0.0f);
}

void OrbitController::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();
	if (input->GetKey(Input::Keys::MOUSE_3))
	{
		if (input->GetKey(Input::Keys::KEY_LEFT_SHIFT))
		{
			this->Pan(input->MouseXDelta(), input->MouseYDelta());
		}
		else
		{
			this->Rotate(input->MouseXDelta(), input->MouseYDelta());
		}
	}

	if (input->ScrollWheel() != 0.0f)
	{
		this->Zoom(input->ScrollWheel());
	}
}

void OrbitController::Rotate(float x, float y)
{
	Transform* transform = this->Owner()->ObjectTransform();

	float rightRotation = y * 0.1f;
	glm::vec3 forward = transform->Forward();

	if (rightRotation < 0 && glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), forward) > -clampRotation ||
		rightRotation > 0 && glm::dot(glm::vec3(0.0f, 1.0f, 0.0f), forward) < clampRotation)
	{
		transform->RotateAroundPoint(orbitPoint, transform->Right(), rightRotation);
	}
	transform->RotateAroundPoint(orbitPoint, transform->Up(), -x * 0.1f);

	glm::vec3 lookDirection = glm::normalize(this->orbitPoint - transform->Position());
	transform->LookAt(lookDirection, glm::vec3(0.0f, 1.0f, 0.0f));
}

void OrbitController::Zoom(float zoomAmount)
{
	Transform* transform = this->Owner()->ObjectTransform();
	glm::vec3 displacement = this->orbitPoint - transform->Position();
	float distance = glm::length(displacement);

	if (distance > maxZoomDistance || zoomAmount < 0.0f)
	{
		glm::vec3 lookDirection = glm::normalize(displacement);
		float zoomSpeed = distance * (float)zoomAmount / zoomSpeedDistanceModifier;
		glm::vec3 newPos = transform->Position() + (lookDirection * zoomSpeed);
		transform->Position(newPos);
	}
}

void OrbitController::Pan(float x, float y)
{
	Transform* transform = this->Owner()->ObjectTransform();

	glm::vec3 displacement = this->orbitPoint - transform->Position();
	float distance = glm::length(displacement);
	float speed = distance * panSpeed / panSpeedDistanceModifier;
	glm::vec3 translation = (transform->Up() * -y * speed) + (transform->Right() * -x * speed);

	transform->Translate(translation);
	this->orbitPoint += translation;
}
