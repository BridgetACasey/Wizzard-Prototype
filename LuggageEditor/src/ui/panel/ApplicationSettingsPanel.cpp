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

        ImGui::Begin("Settings");

        ImGui::TextWrapped("Project functions like saving go here!");

        ImGuiSR::Button("EXIT", ImVec2(120.0f, 80.5f), "exit menu");

		if (openExitMenu)
		{
			if (ImGui::Begin("Exit Menu"))
			{
				ImGui::Text("Quit application?");

				if (ImGuiSR::Button("Yes", ImVec2(240.0f, 80.0f)))
				{
					//TODO: Fix message being cut off by exiting app before speech has finished
					ScreenReaderSupport::OutputAll("Exiting editor application.");

					if (!ScreenReaderSupport::IsSpeaking())
						Application::Get().Close();
				}

				ImGui::SameLine();

				if (ImGuiSR::Button("No", ImVec2(240.0f, 80.0f)))
					openExitMenu = false;

				ImGui::End();
			}
		}

        ImGui::End();
    }
}