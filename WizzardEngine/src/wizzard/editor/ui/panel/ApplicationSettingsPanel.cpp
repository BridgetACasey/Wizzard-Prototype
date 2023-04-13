//@BridgetACasey

#include <wzpch.h>

#include "ApplicationSettingsPanel.h"

#include "imgui.h"
#include "wizzard/audio/Audio.h"
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

		static std::string message = "Project settings.";

		if(Application::Get().GetEditorLayer()->GetEnableTutorialMessages())
		{
			auto& tutorialMessage = Application::Get().GetEditorLayer()->GetTutorialMessages().at("ApplicationSettings");

			if (!tutorialMessage.second)
			{
				message = tutorialMessage.first;
				tutorialMessage.second = true;
				Application::Get().GetEditorLayer()->IncrementTutorialMessagesPlayed();
			}
		}
		else
			message = "Project settings.";

		if (shouldTriggerFocus)
		{
			ImGui::SetNextWindowFocus();

			//bool tutorial = Application::Get().GetEditorLayer()->TriggerTutorialMessage("ApplicationSettings");
			//if (!tutorial)
			//ScreenReaderLogger::QueueOutput(message, true, true);

			Audio::Play(Audio::GetEditorAudioSource(WIZ_AUDIO_UIWINDOWCHANGED));
			shouldTriggerFocus = false;
		}

        ImGuiSR::Begin("PROJECT", nullptr, 0, message, true);

		ImGui::SetItemDefaultFocus();

		if(sceneContext->GetState() == SceneState::EDIT)
		{
			if (ImGuiSR::Button("NEW SCENE", ImVec2(400.0f, 80.5f)))
			{
				//SceneSerialiser serializer(sceneContext);
				//serializer.Serialise(ResourcePath::GetResourcePath(SCENE, "Example.wizzard"));
				ScreenReaderLogger::QueueOutput("Opening file explorer...");
				Application::Get().GetEditorLayer()->NewScene();
			}

			if (ImGuiSR::Button("LOAD SCENE", ImVec2(400.0f, 80.5f)))
			{
				//SceneSerialiser serializer(sceneContext);
				//serializer.Deserialise(ResourcePath::GetResourcePath(SCENE, "Example.wizzard"));
				ScreenReaderLogger::QueueOutput("Opening file explorer...");
				Application::Get().GetEditorLayer()->LoadScene();
			}

			if (ImGuiSR::Button("SAVE SCENE", ImVec2(400.0f, 80.5f)))
			{
				//SceneSerialiser serializer(sceneContext);
				//serializer.Serialise(ResourcePath::GetResourcePath(SCENE, "Example.wizzard"));
				ScreenReaderLogger::QueueOutput("Opening file explorer...");
				Application::Get().GetEditorLayer()->SaveScene();
			}

			if (ImGuiSR::Button("SAVE SCENE AS", ImVec2(440.0f, 80.5f)))
			{
				//SceneSerialiser serializer(sceneContext);
				/*
				 *	...
					serializer.Serialise(ResourcePath::GetResourcePath(SCENE, "Example.wizzard"));
				*/
				ScreenReaderLogger::QueueOutput("Opening file explorer...");
				Application::Get().GetEditorLayer()->SaveSceneAs();
			}
		}

		bool tutorial = Application::Get().GetEditorLayer()->GetEnableTutorialMessages();
		if(ImGuiSR::Checkbox("TUTORIAL MESSAGES", &tutorial))
		{
			Application::Get().GetEditorLayer()->SetEnableTutorialMessages(tutorial);
		}

		if (ImGuiSR::Button("PLAY HOT KEY INFO", ImVec2(580.0f, 80.0f)))
		{
			std::string hotKeyInfo = "In Edit Mode: ";
			hotKeyInfo.append("Navigate with arrow keys or TAB. ");
			hotKeyInfo.append("Change window panel with LEFT CONTROL. ");
			hotKeyInfo.append("Toggle the scene camera to lock onto a selection with CAPSLOCK. ");
			hotKeyInfo.append("Zoom in and out with the scene camera using the left and right bracket keys. ");
			hotKeyInfo.append("Select entities or UI elements with SPACE. ");
			hotKeyInfo.append("Change entity transform type with W, E, and R. ");
			hotKeyInfo.append("Enable entity multiselect mode with M. ");
			hotKeyInfo.append("Duplicate an entity with D. ");
			hotKeyInfo.append("Delete an entity with S. ");
			hotKeyInfo.append("Print information about the currently selected object with O. ");
			hotKeyInfo.append("Print information about the relationships between multiple selected objects with P. ");

			hotKeyInfo.append("In Play Mode: ");
			hotKeyInfo.append("Move the player left and right with the A and D keys. Press SPACE to jump. ");

			ScreenReaderLogger::QueueOutput(hotKeyInfo, true, true);
		}

		//TODO: Implement glossary of SFX cues
		//if(ImGuiSR::Button("PLAY AUDIO GLOSSARY", ImVec2(680.0f, 80.0f)))
		//{
		//	/*
		//	 * ...
		//	 */
		//}

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
		auto sfx = Audio::GetEditorAudioSource(WIZ_AUDIO_EDITORSHUTDOWN);
		Audio::Play(sfx);
		return true;
	}
}