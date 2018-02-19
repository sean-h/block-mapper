#pragma once
#include "glm\glm.hpp"
#include <vector>
#include <string>

class ApplicationContext;

class Debug
{
public:
	void Clear();
	void LogToUI(std::string debugString);
	void LogToUI(std::string name, glm::vec3 v);
	void DrawGUI(ApplicationContext* context);
	int ItemCount() const { return debugStrings.size(); }

private:
	std::vector<std::string> debugStrings;
};