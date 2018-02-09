#pragma once
#include "Component.h"
#include "glm\glm.hpp"

class OrbitController : public Component
{
public:
	OrbitController();
	void Update(ApplicationContext* context);

private:
	glm::vec3 orbitPoint;
	const float clampRotation = 0.9f;
	const float maxZoomDistance = 5.0f;
	const float zoomSpeedDistanceModifier = 10.0f;
};