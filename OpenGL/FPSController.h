#pragma once
#include "Component.h"

class FPSController : public Component
{
public:
	FPSController() { }
	std::string Type() const { return "FPSController"; }
	std::unordered_map<std::string, std::string> Serialize() const { return std::unordered_map<std::string, std::string>(); }
	void Update(ApplicationContext* context);
	void OnAttachToEntity();

private:
	const float moveSpeed = 5.0f;
	float lookSensitivity = 0.2f;
	float moveSpeedModifier = 1.0f;
	const float moveSpeedModifierIncrement = 0.2f;
	const float minMoveSpeedModifier = 0.2f;
	const float maxMoveSpeedModifier = 100.0f;

	float pitch = 0.0f;
	float yaw = 0.0f;
};