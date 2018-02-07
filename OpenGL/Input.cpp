#include "Input.h"
#include <algorithm>

bool Input::GetKey(Keys key) const
{
	return std::find(keysDown.begin(), keysDown.end(), key) != keysDown.end();
}

bool Input::GetKeyDown(Keys key) const
{
	return std::find(keysDownThisFrame.begin(), keysDownThisFrame.end(), key) != keysDownThisFrame.end();
}

bool Input::GetKeyUp(Keys key) const
{
	return std::find(keysUpThisFrame.begin(), keysUpThisFrame.end(), key) == keysUpThisFrame.end();
}

void Input::PollWindowInput(GLFWwindow * window)
{
	// Keyboard Input
	for (int i = (int)Input::Keys::KEY_FIRST; i <= (int)Input::Keys::KEY_LAST; i++)
	{
		int state = glfwGetKey(window, i);
		if (state == GLFW_PRESS)
		{
			SetKeyDown((Keys)i);
		}
		else if (state == GLFW_RELEASE)
		{
			SetKeyUp((Keys)i);
		}
	}

	// Mouse Position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	double xoffset = xpos - this->mouseX;
	double yoffset = this->mouseY - ypos;
	this->mouseX = xpos;
	this->mouseY = ypos;
	this->mouseXDelta = xoffset;
	this->mouseYDelta = yoffset;

	// Mouse Buttons
	for (int i = (int)Input::Keys::MOUSE_FIRST; i <= (int)Input::Keys::MOUSE_LAST; i++)
	{
		int state = glfwGetMouseButton(window, i);
		if (state == GLFW_PRESS)
		{
			SetKeyDown((Keys)i);
		}
		else if (state == GLFW_RELEASE)
		{
			SetKeyUp((Keys)i);
		}
	}
}

void Input::SetKeyDown(Keys key)
{
	if (std::find(keysDown.begin(), keysDown.end(), key) == keysDown.end())
	{
		keysDown.push_back(key);
		keysDownThisFrame.push_back(key);
	}
}

void Input::SetKeyUp(Keys key)
{
	// Check if the key was already pressed
	if (std::find(keysDown.begin(), keysDown.end(), key) != keysDown.end())
	{
		if (std::find(keysUpThisFrame.begin(), keysUpThisFrame.end(), key) == keysUpThisFrame.end())
		{
			keysUpThisFrame.push_back(key);
		}

		keysDown.erase(std::remove(keysDown.begin(), keysDown.end(), key), keysDown.end());
	}
}

void Input::EndFrame()
{
	keysDownThisFrame.clear();
	keysUpThisFrame.clear();
	this->mouseXDelta = 0;
	this->mouseYDelta = 0;
}
