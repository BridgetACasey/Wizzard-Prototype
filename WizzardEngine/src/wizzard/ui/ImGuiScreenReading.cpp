//@BridgetACasey

#include "wzpch.h"

#define IMGUI_DEFINE_MATH_OPERATORS	//TODO: Move this to a more appropriate location

#include <comdef.h>
#include "imgui.h"

#include "ImGuiScreenReading.h"

#include "ImGuiCustom.h"
#include "core/Application.h"
#include "screenreading/ScreenReaderSupport.h"

namespace Wizzard
{
	bool ImGuiSR::WindowBegin(const std::string& windowLabel, bool* isOpen, int flags, const std::string& description, bool preferDesc)
	{
		WIZ_PROFILE_FUNCTION();

		bool success = ImGui::Begin(windowLabel.c_str(), isOpen, flags);

		ImGuiLayer* imguiLayer = Application::Get().GetImGuiLayer();

		if (imguiLayer->GetLogWindowMessage())
		{
			if (imguiLayer->GetUIWindowMessageID() == ImGui::GetCurrentWindow()->ID)
			{
				if (preferDesc && !description.empty())
					ScreenReaderSupport::OutputAll(description);
				else if (!preferDesc)
					ScreenReaderSupport::OutputAll(windowLabel);

				imguiLayer->SetLogWindowMessage(false);
			}
		}

		return success;
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
					ScreenReaderSupport::OutputAll(description);
				else if (!preferDesc)
					ScreenReaderSupport::OutputAll(buttonLabel);

				imguiLayer->SetLogElementMessage(false);
			}
		}

		return pressed;
	}
}