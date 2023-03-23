//@BridgetACasey

#include <wzpch.h>

#include "ApplicationSettingsPanel.h"

#include "imgui.h"
#include "wizzard/base/ResourcePathFinder.h"
#include "wizzard/common/Application.h"
#include "wizzard/scene/SceneSerialiser.h"
#include "wizzard/editor/ui/screenreading/ScreenReaderLogger.h"
#include "wizzard/editor/ui/imgui/ImGuiScreenReading.h"

#include "wizzard/event/MulticastDelegate.h"

namespace Wizzard
{
	ApplicationSettingsPanel::ApplicationSettingsPanel()
	{
		Application::Get().GetAppShutdownDelegate().Bind(WIZ_BIND_EVENT_FN(ApplicationSettingsPanel::OnAppShutdown));
	}

	void ApplicationSettingsPanel::OnEvent(Event& event)
	{
		//EventHandler eventHandler(event);
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
		{
			openExitMenu = !openExitMenu;

			if (openExitMenu)
			{
				ImGuiIO io = ImGui::GetIO();
				ImGui::SetNextWindowPos(ImVec2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
			}
		}

		if (openExitMenu)
		{
			ImGuiWindowFlags exitFlags = ImGuiWindowFlags_NoResize;

			if (ImGuiSR::Begin("EXIT MENU", nullptr, exitFlags, "Quit application?", true))
			{
				ImGui::Text("Quit application?");

				if (ImGuiSR::Button("Yes", ImVec2(240.0f, 80.0f), "Yes, I want to exit", true))
				{
					AppShutdownEvent appEvent;
					Application::Get().GetAppShutdownDelegate().TriggerAll(appEvent);
					//TODO: Fix message being cut off by exiting app before speech has finished
				}

				ImGui::SameLine();

				if (ImGuiSR::Button("No", ImVec2(240.0f, 80.0f), "No, I want to continue", true))
					openExitMenu = false;

				ImGui::End();
			}
		}

        ImGui::End();
    }

	bool ApplicationSettingsPanel::OnAppShutdown(AppShutdownEvent& appEvent)
	{
		WIZ_TRACE("Test message from app panel");
		return true;
	}
}