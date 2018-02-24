#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>

class ApplicationContext;

class FileSelector
{
public:
	FileSelector(std::vector<std::string> filenames, std::function<void(std::string)> acceptFunction, std::function<void()> cancelFunction);
	~FileSelector();
	void DrawGUI();

private:
	int selectedItem;
	const char** listItems;
	std::vector<std::string> items;
	int itemCount;
	std::function<void(std::string)> acceptFunction;
	std::function<void()> cancelFunction;
};
