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
}