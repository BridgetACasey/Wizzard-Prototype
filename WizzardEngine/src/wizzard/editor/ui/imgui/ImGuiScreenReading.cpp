//@BridgetACasey

#include "wzpch.h"

#define IMGUI_DEFINE_MATH_OPERATORS	//TODO: Move this to a more appropriate location

#include <comdef.h>
#include "imgui.h"
#include "ImGuizmo.h"

#include "ImGuiScreenReading.h"

#include "imgui_internal.h"
#include "audio/Audio.h"
#include "audio/AudioSource.h"
#include "base/ResourcePathFinder.h"
#include "common/Application.h"
#include "editor/ui/screenreading/ScreenReaderLogger.h"

//TODO: Refactor a lot of these functions, using too much duplicate code

namespace Wizzard
{
	static AudioSource clickSFX;

	void ImGuiSR::Init()
	{
		if (Audio::IsActive())
		{
			clickSFX = AudioSource::LoadFromFile(ResourcePath::GetResourcePath(SFX, "Logitech-mouse-click.mp3"), false);
		}
		else
			WIZ_ERROR("Could not initialise SFX for ImGuiScreenReading!");
	}

	void ImGuiSR::Shutdown()
	{
		if(clickSFX.IsLoaded())
		clickSFX.FreeSource();
	}

	bool ImGuiSR::Begin(const std::string& windowLabel, bool* isOpen, int flags, const std::string& description, bool preferDesc)
	{
		WIZ_PROFILE_FUNCTION();

		bool success = ImGui::Begin(windowLabel.c_str(), isOpen, flags);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogWindowMessage())
		{
			if(imguiLayer->GetUIWindowMessageID() == ImGui::GetCurrentWindow()->ID)
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description, true, true);
				else if (!preferDesc)
					ScreenReaderLogger::QueueOutput(windowLabel, true, true);

				imguiLayer->SetLogWindowMessage(false);
			}
		}

		return success;
	}

	bool ImGuiSR::End()
	{
		ImGui::End();

		return true;
	}

	bool ImGuiSR::Button(const std::string& buttonLabel, const ImVec2& sizeArg, const std::string& description, bool preferDesc)
	{
		WIZ_PROFILE_FUNCTION();

		bool pressed = ImGui::Button(buttonLabel.c_str(), sizeArg);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogElementMessage())
		{
			if (imguiLayer->GetUIElementMessageID() == ImGui::GetItemID())
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description);
				else if (!preferDesc)
					ScreenReaderLogger::QueueOutput(buttonLabel);

				imguiLayer->SetLogElementMessage(false);
			}
		}

		if(pressed)
			Audio::Play(clickSFX);

		return pressed;
	}

	bool ImGuiSR::Checkbox(const std::string& label, bool* flags, const std::string& description, bool preferDesc)
	{
		WIZ_PROFILE_FUNCTION();

		bool pressed = ImGui::Checkbox(label.c_str(), flags);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		std::string flagStatus = (*flags) ? " ON" : " OFF";

		if (imguiLayer->GetLogElementMessage())
		{
			if (imguiLayer->GetUIElementMessageID() == ImGui::GetItemID())
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description);
				else if (!preferDesc)
					ScreenReaderLogger::QueueOutput(label + flagStatus);

				imguiLayer->SetLogElementMessage(false);
			}
		}

		if (pressed)
		{
			Audio::Play(clickSFX);
			ScreenReaderLogger::QueueOutput(label + flagStatus);
		}

		return pressed;
	}

	bool ImGuiSR::MenuItem(const std::string& label, const char* shortcut, bool selected, bool enabled, const std::string& description, bool preferDesc)
	{
		bool pressed = ImGui::MenuItem(label.c_str());

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogElementMessage())
		{
			if (imguiLayer->GetUIElementMessageID() == ImGui::GetItemID())
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description);
				else if (!preferDesc)
					ScreenReaderLogger::QueueOutput(label);

				imguiLayer->SetLogElementMessage(false);
			}
		}

		if (pressed)
			Audio::Play(clickSFX);

		return pressed;
	}

	bool ImGuiSR::InputText(const char* label, char* buf, size_t buf_size, int flags, int callback, void* user_data, const std::string& description, bool preferDesc)
	{
		bool focused = ImGui::InputText(label, buf, buf_size, flags, reinterpret_cast<ImGuiInputTextCallback>(callback), user_data);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogElementMessage())
		{
			if (imguiLayer->GetUIElementMessageID() == ImGui::GetItemID())
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description);
				else if (!preferDesc)
					ScreenReaderLogger::QueueOutput(label);

				imguiLayer->SetLogElementMessage(false);
			}
		}

		if (focused)
			Audio::Play(clickSFX);

		return focused;
	}

	bool ImGuiSR::TreeNodeEx(const char* label, const void* ptr_id, int flags, const char* fmt, const std::string& description, bool preferDesc, ...)
	{
		va_list args;
		va_start(args, preferDesc);
		bool opened = ImGui::TreeNodeEx(ptr_id, flags, fmt, args);
		va_end(args);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogElementMessage())
		{
			if (imguiLayer->GetUIElementMessageID() == ImGui::GetItemID())
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description);
				else if (!preferDesc)
					ScreenReaderLogger::QueueOutput(label);

				imguiLayer->SetLogElementMessage(false);
			}
		}

		//if (opened)
		//	Audio::Play(clickSFX);

		return opened;
	}

	bool ImGuiSR::BeginCombo(const char* label, const char* preview_value, int flags, const std::string& description, bool preferDesc)
	{
		bool open = ImGui::BeginCombo(label, preview_value, flags);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogElementMessage())
		{
			if (imguiLayer->GetUIElementMessageID() == ImGui::GetItemID())
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description + ": " + preview_value);
				else if (!preferDesc)
				{
					std::string modifiedLabel = label;
					modifiedLabel.append(": ");
					modifiedLabel.append(preview_value);
					ScreenReaderLogger::QueueOutput(modifiedLabel);
				}

				imguiLayer->SetLogElementMessage(false);
			}
		}

		return open;
	}

	bool ImGuiSR::Selectable(const char* label, const ImVec2& size, bool selected, int flags, const std::string& description, bool preferDesc)
	{
		bool select = ImGui::Selectable(label, selected, flags, size);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogElementMessage())
		{
			if (imguiLayer->GetUIElementMessageID() == ImGui::GetItemID())
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description);
				else if (!preferDesc)
					ScreenReaderLogger::QueueOutput(label);

				imguiLayer->SetLogElementMessage(false);
			}
		}

		return select;
	}

	bool ImGuiSR::DragFloat(const char* label, float* v, float v_speed, float v_min, float v_max, const char* format, int flags, const std::string& description, bool preferDesc)
	{
		bool valueChanged = ImGui::DragFloat(label, v, v_speed, v_min, v_max, format, flags);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogElementMessage())
		{
			if (imguiLayer->GetUIElementMessageID() == ImGui::GetItemID())
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description + ": " + std::to_string(v[0]));
				else if (!preferDesc)
				{
					std::string modifiedLabel = label;
					modifiedLabel.append(": " + std::to_string(v[0]));
					ScreenReaderLogger::QueueOutput(modifiedLabel);
				}

				imguiLayer->SetLogElementMessage(false);
			}
		}

		return valueChanged;
	}

	bool ImGuiSR::DragFloat2(const char* label, float v[2], float v_speed, float v_min, float v_max, const char* format, int flags, const std::string& description, bool preferDesc)
	{
		bool valueChanged = ImGui::DragFloat2(label, v, v_speed, v_min, v_max, format, flags);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogElementMessage())
		{
			if (imguiLayer->GetUIElementMessageID() == ImGui::GetItemID())
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description + ": X: " + std::to_string(v[0]) + " Y: " + std::to_string(v[1]));
				else if (!preferDesc)
				{
					std::string modifiedLabel = label;
					modifiedLabel.append(": X: " + std::to_string(v[0]) + " Y: " + std::to_string(v[1]));
					ScreenReaderLogger::QueueOutput(modifiedLabel);
				}

				imguiLayer->SetLogElementMessage(false);
			}
		}

		return valueChanged;
	}

	bool ImGuiSR::DragFloat3(const char* label, float v[3], float v_speed, float v_min, float v_max, const char* format, int flags, const std::string& description, bool preferDesc)
	{
		bool valueChanged = ImGui::DragFloat3(label, v, v_speed, v_min, v_max, format, flags);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogElementMessage())
		{
			if (imguiLayer->GetUIElementMessageID() == ImGui::GetItemID())
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description + ": X: " + std::to_string(v[0]) + " Y: " + std::to_string(v[1]) + " Z: " + std::to_string(v[2]));
				else if (!preferDesc)
				{
					std::string modifiedLabel = label;
					modifiedLabel.append(": X: " + std::to_string(v[0]) + " Y: " + std::to_string(v[1]) + " Z: " + std::to_string(v[2]));
					ScreenReaderLogger::QueueOutput(modifiedLabel);
				}

				imguiLayer->SetLogElementMessage(false);
			}
		}

		return valueChanged;
	}

	bool ImGuiSR::DragFloat4(const char* label, float v[4], float v_speed, float v_min, float v_max, const char* format, int flags, const std::string& description, bool preferDesc)
	{
		bool valueChanged = ImGui::DragFloat4(label, v, v_speed, v_min, v_max, format, flags);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogElementMessage())
		{
			if (imguiLayer->GetUIElementMessageID() == ImGui::GetItemID())
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description + ": X: " + std::to_string(v[0]) + " Y: " + std::to_string(v[1]) + " Z: " + std::to_string(v[2]) + " W: " + std::to_string(v[3]));
				else if (!preferDesc)
				{
					std::string modifiedLabel = label;
					modifiedLabel.append(": X: " + std::to_string(v[0]) + " Y: " + std::to_string(v[1]) + " Z: " + std::to_string(v[2]) + " W: " + std::to_string(v[3]));
					ScreenReaderLogger::QueueOutput(modifiedLabel);
				}

				imguiLayer->SetLogElementMessage(false);
			}
		}

		return valueChanged;
	}

	bool ImGuiSR::ColorEdit4(const char* label, float col[4], int flags, const std::string& description, bool preferDesc)
	{
		bool select = ImGui::ColorEdit4(label, col, flags);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogElementMessage())
		{
			if (imguiLayer->GetUIElementMessageID() == ImGui::GetItemID())
			{
				if (preferDesc && !description.empty())
					ScreenReaderLogger::QueueOutput(description + ": R: " + std::to_string(col[0]) + " G: " + std::to_string(col[1]) + " B: " + std::to_string(col[2]) + " A: " + std::to_string(col[3]));
				else if (!preferDesc)
				{
					std::string modifiedLabel = label;
					modifiedLabel.append(": R: " + std::to_string(col[0]) + " G: " + std::to_string(col[1]) + " B: " + std::to_string(col[2]) + " A: " + std::to_string(col[3]));
					ScreenReaderLogger::QueueOutput(modifiedLabel);
				}

				imguiLayer->SetLogElementMessage(false);
			}
		}

		return select;
	}
}