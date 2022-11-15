//@BridgetACasey

#pragma once

#include "imgui.h"

namespace Wizzard
{
	class ImGuiAccessibility
	{
	public:
		static bool Button(const char* label, const wchar_t* description, bool& readDesc, const ImVec2& size_arg = ImVec2(0, 0));
	};
}