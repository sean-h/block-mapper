#pragma once
#include "Component.h"
#include "glm\glm.hpp"

class Camera : public Component
{
public:
	glm::mat4 ViewMatrix();
	glm::mat4 ProjectionMatrix(float windowWidth, float windowHeight);
	glm::vec3 ScreenToWorldDirection(float screenX, float screenY, float windowWidth, float windowHeight);
	glm::vec3 WorldAxisForward();
	glm::vec3 WorldAxisRight();
	float NearClipDistance() const { return nearClipDistance; }
	float FarClipDistance() const { return farClipDistance; }

private:
	const float verticalFOV = glm::radians(45.0f);
	const float nearClipDistance = 0.1f;
	const float farClipDistance = 1000.0f;
};
