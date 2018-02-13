#pragma once
#include "Window.h"

class ImGuiContext;
class ApplicationContext;

class GUIManager
{
public:
	GUIManager(Window* window);
	~GUIManager();
	void StartFrame();
	void Draw(ApplicationContext* context);

private:
	ImGuiContext* imGuiContext;
};