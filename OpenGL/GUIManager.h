#pragma once
#include "Window.h"

class ImGuiContext;

class GUIManager
{
public:
	GUIManager(Window* window);
	~GUIManager();
	void StartFrame();
	void Draw();

private:
	ImGuiContext* imGuiContext;
};