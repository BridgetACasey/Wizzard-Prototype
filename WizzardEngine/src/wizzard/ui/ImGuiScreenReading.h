//@BridgetACasey

#pragma once

struct ImVec2;

namespace Wizzard
{
	class ImGuiSR
	{
	public:
		static bool Button(const std::string& buttonLabel, const ImVec2& sizeArg, const std::string& description = "", bool preferDesc = false);

		static void SetButtonFontScale(float scale) { buttonFontScale = scale; }
		static float GetButtonFontScale() { return buttonFontScale; }

	private:
		inline static float buttonFontScale = 1.0f;
	};
}