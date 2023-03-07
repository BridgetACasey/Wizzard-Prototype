//@BridgetACasey

#include "ApplicationSettingsPanel.h"

#include <string>
#include "imgui/imgui.h"
#include "wizzard/core/Application.h"
#include "wizzard/screenreading/ScreenReaderSupport.h"
#include "wizzard/ui/ImGuiScreenReading.h"

namespace Wizzard
{
    void ApplicationSettingsPanel::OnImGuiRender()
    {
		static bool openExitMenu = false;

        ImGuiSR::WindowBegin("Settings");

        ImGui::TextWrapped("Project functions like saving go here!");

		if (ImGuiSR::Button("EXIT", ImVec2(161.0f, 80.5f)))
			openExitMenu = !openExitMenu;

		if (openExitMenu)
		{
			if (ImGuiSR::WindowBegin("Exit Menu"))
			{
				ImGui::Text("Quit application?");

				if (ImGuiSR::Button("Yes", ImVec2(240.0f, 80.0f), "Yes, I want to exit", true))
				{
					//TODO: Fix message being cut off by exiting app before speech has finished
					ScreenReaderSupport::OutputAll("Exiting editor application.");

					if (!ScreenReaderSupport::IsSpeaking())
						Application::Get().Close();
				}

				ImGui::SameLine();

				if (ImGuiSR::Button("No", ImVec2(240.0f, 80.0f), "No, I want to continue", true))
					openExitMenu = false;

				ImGui::End();
			}
		}

        ImGui::End();
    }
}