#pragma once
#include "imgui.h"
#include <string>

namespace GUI
{
	bool ToggleButton(std::string text, int id, int & selectedButton, bool fillWidth = true);
}