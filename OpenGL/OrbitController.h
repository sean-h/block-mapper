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
	const float panSpeed = 0.01f;
	const float panSpeedDistanceModifier = 10.0f;

	void Rotate(float x, float y);
	void Zoom(float zoomAmount);
	void Pan(float x, float y);
};