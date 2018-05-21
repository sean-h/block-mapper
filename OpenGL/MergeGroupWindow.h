#pragma once
#include "GUIWindow.h"
#include "Entity.h"
#include "imgui.h"
#include <unordered_map>

class Scene;
class MergeGroup;
class MergeGroupUIItem;

class MergeGroupWindow : public GUIWindow
{
public:
	MergeGroupWindow();
	void Draw(ApplicationContext* context);
	void OnSceneLoaded(ApplicationContext* context);

private:
	void CheckNewMergeGroupName(Scene* scene);

	const int inputTextLength = 128;

	char* newMergeGroupName;
	bool validNewMergeGroupName;
	std::vector<std::unique_ptr<MergeGroupUIItem>> mergeGroupUIItems;
};

class MergeGroupUIItem
{
public:
	MergeGroupUIItem(MergeGroup* mergeGroup, int id);
	void Draw();
	void SetPropertyValue(EntityProperty property, std::string value);

	const int TextLength = 64;

private:
	void DrawPropertyInput(EntityProperty property);

	MergeGroup* mergeGroup;
	int id;
	bool itemCollapsed;
	std::unique_ptr<char> collapseButtonName;
	std::unique_ptr<char> mergeTypeListBoxName;
	std::unordered_map<EntityProperty, std::unique_ptr<char>> inputBuffers;
	std::unordered_map<EntityProperty, std::unique_ptr<char>> inputNames;
};