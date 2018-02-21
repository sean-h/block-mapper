#include "Debug.h"
#include "imgui.h"
#include "ApplicationContext.h"
#include <sstream>
#include <iomanip>

void Debug::Clear()
{
	debugStrings.clear();
}

void Debug::LogToUI(std::string debugString)
{
	debugStrings.push_back(debugString);
}

void Debug::LogToUI(std::string name, glm::vec3 v)
{
	std::stringstream s;
	s << name << std::fixed << std::setprecision(2) << " (X: " << v.x << " Y: " << v.y << " Z: " << v.z << ")";
	debugStrings.push_back(s.str());
}

void Debug::DrawGUI(ApplicationContext * context)
{
	for (auto& item : debugStrings)
	{
		ImGui::Text(item.c_str());
	}
}

void Debug::Render(ApplicationContext * context)
{
	Renderer* renderer = context->ApplicationRenderer();
	for (auto& line : debugLines)
	{
		renderer->DrawLine(context, line.start, line.end);
	}
}

void Debug::DrawLine(glm::vec3 start, glm::vec3 end)
{
	Line line;
	line.start = start;
	line.end = end;
	debugLines.push_back(line);
}
