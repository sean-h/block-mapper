#pragma once
#include "imgui.h"
#include <string>

class ApplicationContext;

class GUIWindow
{
public:
	GUIWindow() { }
	virtual void Open() { open = true; }
	virtual void Close() { open = false; }
	virtual void ToggleOpen() { open = !open; }
	bool* OpenPtr() { return &open; }
	virtual void Draw(ApplicationContext* context) { }
	virtual void OnSceneLoaded(ApplicationContext* context) { }
	std::string Title() const { return title; }
	ImVec2 Position() const { return position; }
	ImVec2 Size() const { return size; }

protected:
	bool open;
	std::string title;
	ImVec2 position;
	ImVec2 size;
};