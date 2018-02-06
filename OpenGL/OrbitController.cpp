#include "OrbitController.h"
#include "Entity.h"
#include "ApplicationContext.h"

OrbitController::OrbitController()
{
	this->orbitDistance = 5.0f;
	this->orbitPoint = glm::vec3(0.0f, 0.0f, 0.0f);
	this->yAxisRotation = 0.0f;
}

void OrbitController::Update(ApplicationContext * context)
{
	yAxisRotation += (float)context->ApplicationTime()->DeltaTime();
	float x = this->orbitDistance * glm::cos(yAxisRotation);
	float z = this->orbitDistance * glm::sin(yAxisRotation);
	glm::vec3 newPos = glm::vec3(x, 0.0f, z);
	this->Owner()->ObjectTransform().Position(newPos);
}
