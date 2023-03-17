//@BridgetACasey

#include <wzpch.h>

#include "ApplicationSettingsPanel.h"

#include "imgui.h"
#include "wizzard/base/ResourcePathFinder.h"
#include "wizzard/common/Application.h"
#include "wizzard/scene/SceneSerialiser.h"
#include "wizzard/editor/ui/screenreading/ScreenReaderLogger.h"
#include "wizzard/editor/ui/imgui/ImGuiScreenReading.h"

namespace Wizzard
{
	void ApplicationSettingsPanel::OnEvent(Event& event)
	{
		EventHandler eventHandler(event);
		eventHandler.HandleEvent<ScreenReaderMessageEndedEvent>(WIZ_BIND_EVENT_FN(ApplicationSettingsPanel::OnScreenReaderMessageEnded));
	}

	void ApplicationSettingsPanel::OnImGuiRender()
    {
		static bool openExitMenu = false;

        ImGuiSR::Begin("PROJECT", nullptr, 0, "Project settings.", true);

		ImGui::SetItemDefaultFocus();

		if (ImGuiSR::Button("SAVE SCENE", ImVec2(400.0f, 80.5f)))
		{
			SceneSerialiser serializer(sceneContext);
			serializer.Serialise(ResourcePath::GetResourcePath(SCENE, "Example.wizzard"));
		}

		//TODO: Re-implement these later along with new/open scene functions
		//if (ImGuiSR::Button("SAVE SCENE AS", ImVec2(440.0f, 80.5f)))
		//{
		//	SceneSerialiser serializer(sceneContext);
		//	/*
		//	 *	...
		//		serializer.Serialise(ResourcePath::GetResourcePath(SCENE, "Example.wizzard"));
		//	*/
		//}
		//
		//if (ImGuiSR::Button("LOAD SCENE", ImVec2(400.0f, 80.5f)))
		//{
		//	SceneSerialiser serializer(sceneContext);
		//	serializer.Deserialise(ResourcePath::GetResourcePath(SCENE, "Example.wizzard"));
		//}

		//TODO: Add SR functions for remaining ImGui elements
		static bool tutorial = true;
		if(ImGuiSR::Checkbox("TUTORIAL MESSAGES", &tutorial))
		{
			/*
			 * ...
			 */
		}

		if(ImGuiSR::Button("PLAY AUDIO GLOSSARY", ImVec2(680.0f, 80.0f)))
		{
			/*
			 * ...
			 */
		}

		if (ImGuiSR::Button("EXIT", ImVec2(161.0f, 80.5f)))
			openExitMenu = !openExitMenu;

		if (openExitMenu)
		{
			ImGuiWindowFlags exitFlags = ImGuiWindowFlags_NoResize;

			if (ImGuiSR::Begin("EXIT MENU", nullptr, exitFlags))
			{
				ImGui::Text("Quit application?");

				if (ImGuiSR::Button("Yes", ImVec2(240.0f, 80.0f), "Yes, I want to exit", true))
				{
					//TODO: Fix message being cut off by exiting app before speech has finished
					ScreenReaderLogger::QueueOutput("Exiting editor application.");

					if (!ScreenReaderLogger::IsSpeaking())
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

	bool ApplicationSettingsPanel::OnScreenReaderMessageEnded(ScreenReaderMessageEndedEvent& srEvent)
	{
		return false;
	}
}