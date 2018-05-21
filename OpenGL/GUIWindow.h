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

	ImVec2 DefaultPosition() const { return defaultPosition; }
	ImVec2 Position() const { return position; }
	void SetPosition(ImVec2 position) { this->position = position; }

	ImVec2 DefaultSize() const { return defaultSize; }
	ImVec2 Size() const { return size; }
	void SetSize(ImVec2 size) { this->size = size; }


protected:
	bool open;
	std::string title;
	ImVec2 defaultPosition;
	ImVec2 defaultSize;
	ImVec2 position;
	ImVec2 size;
};