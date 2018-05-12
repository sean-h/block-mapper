#pragma once
#include "GUIWindow.h"
#include "imgui.h"
#include <unordered_map>

class Scene;

class MergeGroupWindow : public GUIWindow
{
public:
	MergeGroupWindow();
	void Draw(ApplicationContext* context);

private:
	void CheckNewMergeGroupName(Scene* scene);

	const int inputTextLength = 128;

	char* newMergeGroupName;
	bool validNewMergeGroupName;
};