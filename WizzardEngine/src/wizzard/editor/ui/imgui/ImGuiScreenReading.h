//@BridgetACasey

#pragma once

#include <string>

struct ImVec2;

namespace Wizzard
{
	class AudioSource;

	class ImGuiSR
	{
	public:
		static void Init();
		static void Shutdown();

		static bool Begin(const std::string& windowLabel, bool* isOpen = nullptr, int flags = 0, const std::string& description = "", bool preferDesc = false);
		static bool End();

		static bool Button(const std::string& buttonLabel, const ImVec2& sizeArg, const std::string& description = "", bool preferDesc = false);
		static bool Checkbox(const std::string& label, bool* flags, const std::string& description = "", bool preferDesc = false);
		static bool MenuItem(const std::string& label, const char* shortcut = NULL, bool selected = false, bool enabled = true, const std::string& description = "", bool preferDesc = false);

		static void SetButtonFontScale(float scale) { buttonFontScale = scale; }
		static float GetButtonFontScale() { return buttonFontScale; }

	private:
		inline static float buttonFontScale = 1.0f;
	};
}