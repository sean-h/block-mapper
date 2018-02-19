#include "Debug.h"
#include "imgui.h"
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
