//@BridgetACasey

#pragma once

#include "imgui.h"

namespace Wizzard
{
	class ImGuiAccessibility
	{
	public:
		static bool Button(const char* label, const wchar_t* description, bool& readDesc, const ImVec2& size_arg = ImVec2(0, 0));

		static void SetButtonFontScale(float scale) { buttonFontScale = scale; }
		static float GetButtonFontScale() { return buttonFontScale; }

	private:
		inline static float buttonFontScale = 1.0f;
	};
}