#include "Camera.h"
#include "Transform.h"
#include "Entity.h"

glm::mat4 Camera::GetViewMatrix()
{
	Transform* transform = this->Owner()->ObjectTransform();
	return glm::lookAt(transform->Position(), transform->Position() + transform->Forward(), transform->Up());
}

glm::vec3 Camera::ScreenToWorldDirection(float screenX, float screenY, float windowWidth, float windowHeight)
{
	glm::mat4 view = GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), windowWidth / windowHeight, 0.1f, 100.0f);

	glm::vec4 screenRay = glm::vec4((2.0f * screenX) / windowWidth - 1.0f,
		1.0f - (2.0f * screenY) / (float)windowHeight,
		-1.0f, 1.0f);

	glm::vec4 rayEye = glm::inverse(projection) * screenRay;
	glm::vec4 rayWorld = glm::inverse(view) * glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f);
	glm::vec3 mouseVector = glm::normalize(glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z));
	return mouseVector;
}
