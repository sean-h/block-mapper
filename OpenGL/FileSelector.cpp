#include "FileSelector.h"
#include "imgui.h"

FileSelector::FileSelector(std::vector<std::string> filenames, std::function<void(std::string)> acceptFunction, std::function<void()> cancelFunction)
	: acceptFunction(acceptFunction), cancelFunction(cancelFunction)
{
	items = filenames;
	itemCount = items.size();

	listItems = new const char*[items.size()];

	for (int i = 0; i < itemCount; i++)
	{
		listItems[i] = items[i].c_str();
	}
}

FileSelector::~FileSelector()
{
	delete listItems;
}

void FileSelector::DrawGUI()
{
	ImGui::ListBox("Files", &selectedItem, listItems, itemCount);

	if (ImGui::Button("Load"))
	{
		acceptFunction(items[selectedItem]);
	}

	if (ImGui::Button("Close"))
	{
		cancelFunction();
	}
}
