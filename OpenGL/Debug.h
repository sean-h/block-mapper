#pragma once
#include "glm\glm.hpp"
#include <vector>
#include <string>

class ApplicationContext;

struct Line
{
	glm::vec3 start;
	glm::vec3 end;
};

class Debug
{
public:
	void Clear();
	void LogToUI(std::string debugString);
	void LogToUI(std::string name, glm::vec3 v);
	void DrawGUI(ApplicationContext* context);
	void Render(ApplicationContext* context);
	int ItemCount() const { return debugStrings.size(); }
	void DrawLine(glm::vec3 start, glm::vec3 end);

private:
	std::vector<std::string> debugStrings;
	std::vector<Line> debugLines;
};