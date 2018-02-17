#pragma once
#include "Component.h"
#include "glm\glm.hpp"

class Camera : public Component
{
public:
	glm::mat4 GetViewMatrix();
	glm::vec3 ScreenToWorldDirection(float screenX, float screenY, float windowWidth, float windowHeight);
	glm::vec3 WorldAxisForward();
	glm::vec3 WorldAxisRight();
};
