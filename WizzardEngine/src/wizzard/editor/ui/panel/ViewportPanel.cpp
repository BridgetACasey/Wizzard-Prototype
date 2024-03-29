//@BridgetACasey

#include "wzpch.h"

#include "ViewportPanel.h"

#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_internal.h"
#include "wizzard/base/ResourcePathFinder.h"
#include "wizzard/editor/EntitySelection.h"
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

	void ViewportPanel::OnUpdate(TimeStep timeStep)
	{
		if (sceneContext->GetState() == SceneState::PLAY)
			displayFPS = 1.0f / timeStep;
		else
			displayFPS = 0.0f;

		if (Input::IsKeyUp(Key::Left))
			if (Input::IsKeyUp(Key::Right))
				if (Input::IsKeyUp(Key::Up))
					if (Input::IsKeyUp(Key::Down))
						isTransformActive = false;

		static float timer = 0.0f;
		if (isTransformActive)
		{
			//timer = (timer > 0.5f) ? 0.0f : timer + timeStep;

			if (timer > 0.1f)
			{
				timer = 0.0f;
				canTriggerTransform = true;
			}
			else
			{
				timer += timeStep;
				canTriggerTransform = false;
			}
		}
		else
			timer = 0.0f;
	}

	void ViewportPanel::OnEvent(Event& event)
	{
		EventHandler eventHandler(event);

		eventHandler.HandleEvent<KeyPressedEvent>(WIZ_BIND_EVENT_FN(OnKeyPressed));
	}

	void ViewportPanel::OnImGuiRender()
	{
		static ImGuiWindowFlags viewportFlags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize;

		static std::string message = "Viewport.";

		if (Application::Get().GetEditorLayer()->GetEnableTutorialMessages())
		{
			auto& tutorialMessage = Application::Get().GetEditorLayer()->GetTutorialMessages().at("Viewport");

			if (!tutorialMessage.second)
			{
				message = tutorialMessage.first;
				tutorialMessage.second = true;
				Application::Get().GetEditorLayer()->IncrementTutorialMessagesPlayed();
			}
		}
		else
			message = "Viewport.";

		if (shouldTriggerFocus)
		{
			ImGui::SetNextWindowFocus();

			//bool tutorial = Application::Get().GetEditorLayer()->TriggerTutorialMessage("Viewport");
			//if(!tutorial)
				//ScreenReaderLogger::QueueOutput(message, true, true);

			Audio::Play(Audio::GetEditorAudioSource(WIZ_AUDIO_UIWINDOWCHANGED));
		}

		ImGuiSR::Begin("VIEWPORT", nullptr, viewportFlags, message, true);

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
		auto viewportOffset = ImGui::GetWindowPos();

		viewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		viewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };
		
		isFocused = ImGui::IsWindowFocused();
		isHovered = ImGui::IsWindowHovered();

		bool enableCamera = Application::Get().GetEditorLayer()->GetLockOnSelection() ? (EntitySelection::GetSelections().empty() ? true : false) : true;

		Application::Get().GetEditorLayer()->GetEditorCamera().SetEnableUserControl(isHovered && isFocused && enableCamera);
		Application::Get().GetEditorLayer()->GetEditorCamera().SetZoomOnlyControl(isHovered && isFocused);
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

		if (isFocused && Application::Get().GetEditorLayer()->GetEditorCamera().GetEnableUserControl() && Application::Get().GetEditorLayer()->GetLockOnSelection())
			Input::SetMousePosition(viewportOffset.x + (viewportSize.x / 2.0f), viewportOffset.y + (viewportSize.y / 2.0f));	//TODO: Ensure this is centred even on resize
		
		//Gizmos
		if (sceneContext->GetState() == SceneState::EDIT)
		{
			//Entity selectedEntity = Application::Get().GetEditorLayer()->GetSelectedEntity();

			if (gizmoType != -1)
			{
				for (auto selectedEntity : EntitySelection::GetSelections())
				{
					if (selectedEntity)
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

						switch(gizmoType)
						{
							case ImGuizmo::OPERATION::TRANSLATE:
								snapValue = 0.5f;
								break;
							case ImGuizmo::OPERATION::ROTATE:
								snapValue = 10.0f;
								break;
							case ImGuizmo::OPERATION::SCALE:
								snapValue = 0.1f;
								break;
							default:
								snapValue = 1.0f;
						}

						// Snap to 45 degrees for rotation
						//if (gizmoType == ImGuizmo::OPERATION::ROTATE)
						//	snapValue = 45.0f;
						//else
						//	snapValue = 0.5f;

						float snapValues[3] = { snapValue, snapValue, snapValue };

						ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
							(ImGuizmo::OPERATION)gizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
							nullptr, snap ? snapValues : nullptr);

						if (Application::Get().GetEditorLayer()->GetLockOnSelection())
						{
							if (isHovered && isFocused && !enableCamera && canTriggerTransform)
							{
								bool updated = false;

								if (gizmoType == ImGuizmo::OPERATION::TRANSLATE)
								{
									if (Input::IsKeyDown(Key::Left))
									{
										tc.Translation.x -= snapValue;
										updated = true;
									}
									if (Input::IsKeyDown(Key::Right))
									{
										tc.Translation.x += snapValue;
										updated = true;
									}
									if (Input::IsKeyDown(Key::Up))
									{
										tc.Translation.y += snapValue;
										updated = true;
									}
									if (Input::IsKeyDown(Key::Down))
									{
										tc.Translation.y -= snapValue;
										updated = true;
									}
								}

								else if (gizmoType == ImGuizmo::OPERATION::ROTATE)
								{
									if (Input::IsKeyDown(Key::Left))
									{
										tc.Rotation.z -= snapValue;
										updated = true;
									}
									if (Input::IsKeyDown(Key::Right))
									{
										tc.Rotation.z += snapValue;
										updated = true;
									}
								}

								else if (gizmoType == ImGuizmo::OPERATION::SCALE)
								{
									if (Input::IsKeyDown(Key::Up))
									{
										tc.Scale += snapValue;
										updated = true;
									}
									if (Input::IsKeyDown(Key::Down))
									{
										tc.Scale -= snapValue;
										updated = true;
									}
								}

								if (updated)
								{
									Application::Get().GetEditorLayer()->GetEditorCamera().FocusOnPoint(tc.Translation);

									AudioSource& moveSFX = Audio::GetEditorAudioSource(WIZ_AUDIO_ENTITYMOVED);

									if (gizmoType == ImGuizmo::TRANSLATE)
									{
										glm::vec3 pos = entityBaseTranslation - tc.Translation;
										moveSFX.SetPosition(pos.x, pos.y, pos.z);
										float mag = sqrtf((pos.x * pos.x) + (pos.y * pos.y) + (pos.z * pos.z));
										moveSFX.SetPitch(abs(mag));

										if (!EntitySelection::IsMultiSelect())
										{
											moveUnitCount++;

											if (moveUnitCount % 25 == 0)
												ScreenReaderLogger::QueueOutput(selectedEntity.GetName() + " at X " + std::to_string((int)tc.Translation.x) + ", Y " + std::to_string((int)tc.Translation.y));
										}
									}
									else
									{
										moveSFX.SetPosition(0.0f, 0.0f, 0.0f);

										if(gizmoType == ImGuizmo::ROTATE)
											moveSFX.SetPitch(1.0f);
										else
										{
											float baseMag = sqrtf((entityBaseScale.x * entityBaseScale.x) + (entityBaseScale.y * entityBaseScale.y) + (entityBaseScale.z * entityBaseScale.z));
											float newMag = sqrtf((tc.Scale.x * tc.Scale.x) + (tc.Scale.y * tc.Scale.y) + (tc.Scale.z * tc.Scale.z));
											float pitch = (baseMag > newMag) ? moveSFX.GetPitch() + 0.05f : moveSFX.GetPitch() - 0.05f;
											moveSFX.SetPitch(pitch > 0.0f ? pitch : 0.2f);

											entityBaseScale = tc.Scale;
										}
									}

									Audio::Play(moveSFX);
								}
							}
						}
						else if (ImGuizmo::IsUsing())
						{
							glm::vec3 translation, rotation, scale;
							Maths::DecomposeTransform(transform, translation, rotation, scale);

							glm::vec3 deltaRotation = rotation - tc.Rotation;
							tc.Translation = translation;
							tc.Rotation += deltaRotation;
							tc.Scale = scale;
						}
					}
				}
			}
		}

		ImGui::End();	//End Viewport

		ImGuiSR::Begin("##debug", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
		std::string fps = "FPS: ";
		fps.append(std::to_string(displayFPS));
		ImGui::Text(fps.c_str());
		ImGuiSR::End();
	}

	bool ViewportPanel::OnKeyPressed(KeyPressedEvent& keyEvent)
	{
		if (keyEvent.GetKeyCode() == Key::Left || keyEvent.GetKeyCode() == Key::Right || keyEvent.GetKeyCode() == Key::Up || keyEvent.GetKeyCode() == Key::Down)
			if(!EntitySelection::GetSelections().empty())
			isTransformActive = true;

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
		static std::string message = "Viewport toolbar.";

		if (Application::Get().GetEditorLayer()->GetEnableTutorialMessages())
		{
			auto& tutorialMessage = Application::Get().GetEditorLayer()->GetTutorialMessages().at("ViewportToolbar");

			if (!tutorialMessage.second)
			{
				message = tutorialMessage.first;
				tutorialMessage.second = true;
				Application::Get().GetEditorLayer()->IncrementTutorialMessagesPlayed();
			}
		}
		else
			message = "Viewport toolbar.";

		if (shouldTriggerFocus)
		{
			ImGui::SetNextWindowFocus();

			//if(Application::Get().GetEditorLayer()->GetEnableTutorialMessages() && sceneContext->GetState() == SceneState::PLAY)
			//{
			//	auto playTutorial = Application::Get().GetEditorLayer()->GetTutorialMessages().at("PlayModeEntered");
			//	message = playTutorial.first;
			//}
			//else
			//	message = "Viewport toolbar.";

			ScreenReaderLogger::QueueOutput("Cycled to " + message, true, true);

			Audio::Play(Audio::GetEditorAudioSource(WIZ_AUDIO_UIWINDOWCHANGED));
			shouldTriggerFocus = false;
		}

		ImGuiSR::Begin("##toolbar", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse, message, true);

		//if(sceneContext->GetState() == SceneState::PLAY)
		//	ImGui::SetKeyboardFocusHere();

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