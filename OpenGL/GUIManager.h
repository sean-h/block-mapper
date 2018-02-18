#pragma once
#include "Window.h"
#include <unordered_map>
#include <string>

class ImGuiContext;
class ApplicationContext;

struct GUILocation
{
	GUILocation();
	GUILocation(float xPosition, float yPosition, float width, float height)
		: xPosition(xPosition), yPosition(yPosition), width(width), height(height) {}

	float xPosition;
	float yPosition;
	float width;
	float height;
};

class GUIManager
{
public:
	GUIManager(Window* window);
	~GUIManager();
	void StartFrame();
	void Draw(ApplicationContext* context);
	bool MouseOverGUIElement(float mouseX, float mouseY);
	GUILocation WindowLocation(std::string windowName);

private:
	ImGuiContext* imGuiContext;
	std::unordered_map<std::string, GUILocation> windowLocations;
	bool mainMenuOpen;
};