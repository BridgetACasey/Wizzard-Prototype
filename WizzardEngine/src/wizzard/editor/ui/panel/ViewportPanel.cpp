//@BridgetACasey

#include "wzpch.h"

#include "ViewportPanel.h"

#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_internal.h"
#include "wizzard/audio/Audio.h"
#include "glm/gtc/type_ptr.hpp"
#include "wizzard/base/Maths.h"
#include "wizzard/scene/component/TransformComponent.h"
#include "wizzard/editor/ui/imgui/ImGuiScreenReading.h"
#include "wizzard/common/Application.h"
#include "wizzard/input/Input.h"

namespace Wizzard
{
	ViewportPanel::~ViewportPanel()
	{
	}

	void ViewportPanel::OnEvent(Event& event)
	{
	}

	void ViewportPanel::OnImGuiRender()
	{
		static ImGuiWindowFlags viewportFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;

		if (shouldTriggerFocus)
		{
			ImGui::SetNextWindowFocus();
			ScreenReaderLogger::ForceQueueOutput("VIEWPORT");
			Audio::Play(Audio::GetEditorAudioSource(WIZ_AUDIO_UIWINDOWCHANGED));
		}

		ImGuiSR::Begin("VIEWPORT", nullptr, viewportFlags);

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();

		viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
		
		isFocused = ImGui::IsWindowFocused();
		isHovered = ImGui::IsWindowHovered();

		Application::Get().GetEditorLayer()->GetEditorCamera().SetEnableUserControl(isHovered && isFocused);
		Application::Get().GetImGuiLayer()->BlockImGuiEvents(false);
		////Application::Get().GetImGuiLayer()->BlockImGuiEvents(isViewportHovered && isViewportFocused);

		if(shouldTriggerFocus)
		{
			Input::SetMousePosition(viewportOffset.x + (viewportSize.x / 2.0f), viewportOffset.y + (viewportSize.y / 2.0f));
			shouldTriggerFocus = false;
		}
		
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		viewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		
		uint64_t textureID = Application::Get().GetEditorLayer()->GetFrameBuffer()->GetColorAttachmentRendererID();
		ImGui::Image(reinterpret_cast<void*>(textureID), ImVec2{ viewportSize.x, viewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
				
		if (Application::Get().GetEditorLayer()->GetEditorCamera().GetEnableUserControl() && Application::Get().GetEditorLayer()->GetLockOnSelection())
			Input::SetMousePosition(viewportOffset.x + (viewportSize.x / 2.0f), viewportOffset.y + (viewportSize.y / 2.0f));	//TODO: Ensure this is centred even on resize
		
		//Gizmos
		Entity selectedEntity = Application::Get().GetEditorLayer()->GetSelectedEntity();
		if (selectedEntity && gizmoType != -1)
		{
			ImGuizmo::SetOrthographic(true);
			ImGuizmo::SetDrawlist();
		
			ImGuizmo::SetRect(viewportBounds[0].x, viewportBounds[0].y, viewportBounds[1].x - viewportBounds[0].x, viewportBounds[1].y - viewportBounds[0].y);
		
			//Application::Get().GetEditorLayer()->GetEditorCamera().FocusOnPoint(selectedEntity.GetComponent<TransformComponent>().Translation);
		
			// Editor camera
			const glm::mat4& cameraProjection = Application::Get().GetEditorLayer()->GetEditorCamera().GetProjection();
			glm::mat4 cameraView = Application::Get().GetEditorLayer()->GetEditorCamera().GetViewMatrix();
		
			// Entity transform
			auto& tc = selectedEntity.GetComponent<TransformComponent>();
			glm::mat4 transform = tc.GetTransform();
		
			// Snapping
			bool snap = true;//Input::IsKeyDown(Key::LeftControl);
		
			float snapValue = 0.5f; // Snap to 0.5m for translation/scale
			// Snap to 45 degrees for rotation
			if (gizmoType == ImGuizmo::OPERATION::ROTATE)
				snapValue = 45.0f;
		
			float snapValues[3] = { snapValue, snapValue, snapValue };
		
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, snap ? snapValues : nullptr);
		
			if (ImGuizmo::IsUsing())
			{
				glm::vec3 translation, rotation, scale;
				Maths::DecomposeTransform(transform, translation, rotation, scale);
		
				glm::vec3 deltaRotation = rotation - tc.Rotation;
				tc.Translation = translation;
				tc.Rotation += deltaRotation;
				tc.Scale = scale;
			}
			else
			{
				if (gizmoType == ImGuizmo::OPERATION::TRANSLATE && Application::Get().GetEditorLayer()->GetLockOnSelection())
				{
					tc.Translation.x = Application::Get().GetEditorLayer()->GetEditorCamera().GetPosition().x;
					tc.Translation.y = Application::Get().GetEditorLayer()->GetEditorCamera().GetPosition().y;
				}
		
				else if (gizmoType == ImGuizmo::OPERATION::ROTATE)
				{
					//Rotate object
				}
		
				else if (gizmoType == ImGuizmo::OPERATION::SCALE)
				{
					//Scale object
				}
			}
		}

		ImGui::End();	//End Viewport
	}

	bool ViewportPanel::OnKeyPressed(KeyPressedEvent& keyEvent)
	{
		return false;
	}

	
	ViewportToolbarPanel::~ViewportToolbarPanel()
	{
	}
	void ViewportToolbarPanel::OnEvent(Event& event)
	{
	}
	void ViewportToolbarPanel::OnImGuiRender()
	{
		if (shouldTriggerFocus)
		{
			ImGui::SetNextWindowFocus();
			ScreenReaderLogger::ForceQueueOutput("VIEWPORT TOOLBAR");
			Audio::Play(Audio::GetEditorAudioSource(WIZ_AUDIO_UIWINDOWCHANGED));
			shouldTriggerFocus = false;
		}

		ImGuiSR::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse, "VIEWPORT TOOLBAR", true);

		//ImGui::SetKeyboardFocusHere();

		//Temp??
		static std::string playButtonLabel = "PLAY";
		static std::string playButtonDesc = "Play scene.";

		if (ImGuiSR::Button(playButtonLabel, ImVec2(175.0f, 80.5f), playButtonDesc, true))
		{
			switch (sceneContext->GetState())
			{
			case SceneState::EDIT:
			{
				Application::Get().GetEditorLayer()->OnSceneBeginPlay();
				playButtonLabel = "STOP";
				playButtonDesc = "Stop scene.";
			}
			break;
			case SceneState::PLAY:	//Removed pause functionality for now as it requires further testing
			{
				Application::Get().GetEditorLayer()->OnSceneEndPlay();
				playButtonLabel = "PLAY";
				playButtonDesc = "Play scene.";
			}
			}
		}

		ImGui::SetItemDefaultFocus();

		ImGui::End();
	}
}