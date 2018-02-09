#pragma once
//https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/camera.h

#include <vector>
#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Component.h"
#include "Transform.h"
#include "Entity.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVTY = 0.1f;
const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera : public Component
{
public:
	// Eular Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Constructor with vectors
	Camera()
	{
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		Transform* transform = this->Owner()->ObjectTransform();
		return glm::lookAt(transform->Position(), transform->Position() + transform->Forward(), transform->Up());
	}

	glm::vec3 ScreenToWorldDirection(float screenX, float screenY, float windowWidth, float windowHeight)
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
};
