#include "FPSController.h"
#include "ApplicationContext.h"

void FPSController::Update(ApplicationContext * context)
{
	Input* input = context->ApplicationInput();
	Time* time = context->ApplicationTime();

	if (input->GetKeyDown(Input::Keys::KEY_ESCAPE))
	{
		context->ApplicationScene()->ActiveCamera()->SetOrbitMode(context);
		return;
	}

	if (input->ScrollWheel() != 0)
	{
		this->moveSpeedModifier = glm::clamp(moveSpeedModifier + ((float)input->ScrollWheel() * moveSpeedModifierIncrement), minMoveSpeedModifier, maxMoveSpeedModifier);
	}

	if (input->GetKey(Input::Keys::KEY_W))
	{
		Transform* transform = this->Owner()->ObjectTransform();
		transform->Translate(transform->Forward() * (float)time->DeltaTime() * this->moveSpeed * this->moveSpeedModifier);
	}
	if (input->GetKey(Input::Keys::KEY_S))
	{
		Transform* transform = this->Owner()->ObjectTransform();
		transform->Translate(-transform->Forward() * (float)time->DeltaTime() * this->moveSpeed * this->moveSpeedModifier);
	}
	if (input->GetKey(Input::Keys::KEY_D))
	{
		Transform* transform = this->Owner()->ObjectTransform();
		transform->Translate(transform->Right() * (float)time->DeltaTime() * this->moveSpeed * this->moveSpeedModifier);
	}
	if (input->GetKey(Input::Keys::KEY_A))
	{
		Transform* transform = this->Owner()->ObjectTransform();
		transform->Translate(-transform->Right() * (float)time->DeltaTime() * this->moveSpeed * this->moveSpeedModifier);
	}
	if (input->GetKey(Input::Keys::KEY_C))
	{
		Transform* transform = this->Owner()->ObjectTransform();
		transform->Translate(-transform->Up() * (float)time->DeltaTime() * this->moveSpeed * this->moveSpeedModifier);
	}
	if (input->GetKey(Input::Keys::KEY_SPACE))
	{
		Transform* transform = this->Owner()->ObjectTransform();
		transform->Translate(transform->Up() * (float)time->DeltaTime() * this->moveSpeed * this->moveSpeedModifier);
	}

	lookSensitivity = 0.06f;

	yaw += input->MouseXDelta() * this->lookSensitivity;
	pitch += input->MouseYDelta() * this->lookSensitivity;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	else if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	Transform* transform = this->Owner()->ObjectTransform();
	transform->LookAt(front, glm::vec3(0.0f, 1.0f, 0.0f));
}

void FPSController::OnAttachToEntity()
{
	// Keep camera direction
	glm::vec3 forward = this->Owner()->ObjectTransform()->Forward();
	pitch = glm::degrees(glm::asin(forward.y));
	yaw = glm::degrees(std::atan2(forward.z, forward.x));
}
