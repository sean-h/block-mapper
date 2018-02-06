#pragma once
#include "Component.h"
#include "glm\glm.hpp"

class OrbitController : public Component
{
public:
	OrbitController();
	void Update(ApplicationContext* context);

private:
	float orbitDistance;
	glm::vec3 orbitPoint;
	float yAxisRotation;
};