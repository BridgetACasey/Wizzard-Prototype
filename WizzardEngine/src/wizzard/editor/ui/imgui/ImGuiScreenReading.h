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
		static bool InputText(const char* label, char* buf, size_t buf_size, int flags = 0, int callback = NULL, void* user_data = NULL, const std::string& description = "", bool preferDesc = false);
		static bool TreeNodeEx(const char* label, const void* ptr_id, int flags, const char* fmt, const std::string& description = "", bool preferDesc = false, ...);
		static bool BeginCombo(const char* label, const char* preview_value, int flags = 0, const std::string& description = "", bool preferDesc = false);
		static bool Selectable(const char* label, const ImVec2& size, bool selected = false, int flags = 0, const std::string& description = "", bool preferDesc = false);

		static bool DragFloat(const char* label, float* v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", int flags = 0, const std::string& description = "", bool preferDesc = false);
		static bool DragFloat2(const char* label, float v[2], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", int flags = 0, const std::string& description = "", bool preferDesc = false);
		static bool DragFloat3(const char* label, float v[3], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", int flags = 0, const std::string& description = "", bool preferDesc = false);
		static bool DragFloat4(const char* label, float v[4], float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", int flags = 0, const std::string& description = "", bool preferDesc = false);

		static bool ColorEdit4(const char* label, float col[4], int flags = 0, const std::string& description = "", bool preferDesc = false);

		static void SetButtonFontScale(float scale) { buttonFontScale = scale; }
		static float GetButtonFontScale() { return buttonFontScale; }

	private:
		inline static float buttonFontScale = 1.0f;
	};
}