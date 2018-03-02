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

std::unordered_map<std::string, std::string> OrbitController::Serialize() const
{
	std::unordered_map<std::string, std::string> dataMap;

	dataMap["OrbitPointX"] = std::to_string(orbitPoint.x);
	dataMap["OrbitPointY"] = std::to_string(orbitPoint.y);
	dataMap["OrbitPointZ"] = std::to_string(orbitPoint.z);

	return dataMap;
}

void OrbitController::OnAttachToEntity()
{
	Transform* transform = this->Owner()->ObjectTransform();
	this->orbitPoint = (transform->Position() + transform->Forward() * glm::length(glm::vec2(20.0f, 20.0f)));
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

	if (input->GetKeyDown(Input::Keys::KEY_DELETE))
	{
		glm::vec3 newOrbitPoint;

		int selectedEntityCount = 0;
		for (auto& selectedEntity : context->ApplicationEntitySelectionManager()->SelectedEntities())
		{
			newOrbitPoint += selectedEntity->TargetEntity()->ObjectTransform()->Position();
			selectedEntityCount++;
		}

		if (selectedEntityCount > 0)
		{
			newOrbitPoint /= (float)selectedEntityCount;
			this->orbitPoint = newOrbitPoint;
			this->Rotate(0.0f, 0.0f);
		}
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
