//@BridgetACasey

#include <wzpch.h>

#include "SceneHierarchyPanel.h"

#include <imgui.h>

#include "imgui_internal.h"
#include "wizzard/common/Application.h"
#include "wizzard/audio/Audio.h"
#include "wizzard/event/EventHandler.h"
#include "glm/gtc/type_ptr.hpp"
#include "input/Input.h"
#include "scene/component/AudioListenerComponent.h"
#include "scene/component/BoxCollider2DComponent.h"
#include "scene/component/CameraComponent.h"
#include "scene/component/CharacterControllerComponent.h"
#include "scene/component/RigidBody2DComponent.h"
#include "scene/component/SpriteComponent.h"
#include "scene/component/TransformComponent.h"
#include "wizzard/scene/component/TagComponent.h"
#include "wizzard/editor/ui/imgui/ImGuiScreenReading.h"
#include "wizzard/editor/EntitySelection.h"

namespace Wizzard
{
	void SceneHierarchyPanel::OnEvent(Event& event)
	{
		EventHandler eventHandler(event);

		eventHandler.HandleEvent<KeyPressedEvent>(WIZ_BIND_EVENT_FN(OnKeyPressed));
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
		if (shouldTriggerFocus)
		{
			ImGui::SetNextWindowFocus();
			ScreenReaderLogger::QueueOutput("SCENE HIERARCHY");
			Audio::Play(Audio::GetEditorAudioSource(WIZ_AUDIO_UIWINDOWCHANGED));
			shouldTriggerFocus = false;
		}

		ImGuiSR::Begin("SCENE", nullptr, 0, "Scene hierarchy.", true);

		ImGui::SetItemDefaultFocus();

		if (sceneContext)
		{
			sceneContext->registry.each([&](auto entityID)
			{
				Entity entity{ entityID , sceneContext.Get() };
				DrawEntityNode(entity);

				//TODO: Check if default entity has been deleted from scene, then reset
				//if (defaultEntity.GetName().empty())
				//	defaultEntity = entity;
			});

			// Right-click on blank space
			//TODO: Fix later. Probably use event callbacks instead and pass to editor layer to handle.
			//if (Input::IsKeyPressed(Key::Enter) || Input::IsMouseButtonPressed(Mouse::RightButton))
				

			if(ImGuiSR::Button("CREATE ENTITY", ImVec2(440.0f, 80.5f)))
				openEntityCreationWindow = !openEntityCreationWindow;

			if (openEntityCreationWindow)
			{
				if (ImGuiSR::Begin("Entity Creation"))
				{
					//ImGui::SetKeyboardFocusHere();

					if (ImGuiSR::MenuItem("Create Floor (Short)"))
					{
						auto floor = sceneContext->CreateEntity("Floor (Short)");
						floor.GetComponent<TagComponent>().baseTag = "Floor (Short)";
						floor.AddComponent<SpriteComponent>(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
						floor.GetComponent<TransformComponent>().Translation.x = 0.0f;
						floor.GetComponent<TransformComponent>().Translation.y = 0.0f;
						floor.GetComponent<TransformComponent>().Scale.y *= 0.5f;
						floor.GetComponent<TransformComponent>().Scale.x *= 3.0f;
						floor.AddComponent<RigidBody2DComponent>();
						floor.GetComponent<RigidBody2DComponent>().Type = RigidBody2DComponent::BodyType::Static;
						floor.AddComponent<BoxCollider2DComponent>();
						openEntityCreationWindow = false;
					}

					if (ImGuiSR::MenuItem("Create Floor (Long)"))
					{
						auto floor = sceneContext->CreateEntity("Floor (Long)");
						floor.GetComponent<TagComponent>().baseTag = "Floor (Long)";
						floor.AddComponent<SpriteComponent>(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
						floor.GetComponent<TransformComponent>().Translation.x = 0.0f;
						floor.GetComponent<TransformComponent>().Translation.y = 0.0f;
						floor.GetComponent<TransformComponent>().Scale.y *= 0.5f;
						floor.GetComponent<TransformComponent>().Scale.x *= 6.0f;
						floor.AddComponent<RigidBody2DComponent>();
						floor.GetComponent<RigidBody2DComponent>().Type = RigidBody2DComponent::BodyType::Static;
						floor.AddComponent<BoxCollider2DComponent>();
						openEntityCreationWindow = false;
					}

					if (ImGuiSR::MenuItem("Create Wall (Short)"))
					{
						auto wall = sceneContext->CreateEntity("Wall (Short)");
						wall.GetComponent<TagComponent>().baseTag = "Wall (Short)";
						wall.AddComponent<SpriteComponent>(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
						wall.GetComponent<TransformComponent>().Translation.x = 0.0f;
						wall.GetComponent<TransformComponent>().Translation.y = 0.0f;
						wall.GetComponent<TransformComponent>().Scale.y *= 1.5f;
						wall.GetComponent<TransformComponent>().Scale.x *= 0.5f;
						wall.AddComponent<RigidBody2DComponent>();
						wall.GetComponent<RigidBody2DComponent>().Type = RigidBody2DComponent::BodyType::Static;
						wall.AddComponent<BoxCollider2DComponent>();
						openEntityCreationWindow = false;
					}

					if (ImGuiSR::MenuItem("Create Wall (Tall)"))
					{
						auto wall = sceneContext->CreateEntity("Wall (Tall)");
						wall.GetComponent<TagComponent>().baseTag = "Wall (Tall)";
						wall.AddComponent<SpriteComponent>(glm::vec4{ 1.0f, 1.0f, 1.0f, 1.0f });
						wall.GetComponent<TransformComponent>().Translation.x = 0.0f;
						wall.GetComponent<TransformComponent>().Translation.y = 0.0f;
						wall.GetComponent<TransformComponent>().Scale.y *= 3.0f;
						wall.GetComponent<TransformComponent>().Scale.x *= 0.5f;
						wall.AddComponent<RigidBody2DComponent>();
						wall.GetComponent<RigidBody2DComponent>().Type = RigidBody2DComponent::BodyType::Static;
						wall.AddComponent<BoxCollider2DComponent>();
						openEntityCreationWindow = false;
					}

					if (ImGuiSR::MenuItem("Create Player Character"))
					{
						if (!sceneContext->GetPrimaryCameraEntity())
						{
							auto player = sceneContext->CreateEntity("Player Character");
							player.GetComponent<TagComponent>().baseTag = "Player Character";
							player.AddComponent<SpriteComponent>(glm::vec4{ 0.0f, 1.0f, 0.0f, 1.0f });
							player.GetComponent<TransformComponent>().Translation.x = 0.0f;
							player.GetComponent<TransformComponent>().Translation.y = 0.0f;
							player.AddComponent<RigidBody2DComponent>();
							player.GetComponent<RigidBody2DComponent>().Type = RigidBody2DComponent::BodyType::Dynamic;
							player.AddComponent<BoxCollider2DComponent>();
							player.AddComponent<CameraComponent>();
							player.AddComponent<CharacterControllerComponent>();
							openEntityCreationWindow = false;
						}
						else
							ScreenReaderLogger::QueueOutput("Scene already contains player entity called: " + sceneContext->GetPrimaryCameraEntity().GetName());
					}

					if (ImGuiSR::MenuItem("Create Empty Entity"))
					{
						sceneContext->CreateEntity("Empty Entity");
						openEntityCreationWindow = false;
					}


					if (ImGuiSR::MenuItem("Close Window"))
					{
						ScreenReaderLogger::QueueOutput("Closed entity creation window");
						openEntityCreationWindow = false;
					}

					ImGuiSR::End();
				}
			}

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			{
				EntitySelection::DeselectAll();
				Application::Get().GetEditorLayer()->GetPropertiesPanel()->SetSelectedEntity({});
				ScreenReaderLogger::QueueOutput("Deselected all");
			}
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = !EntitySelection::GetSelections().empty() ? ((EntitySelection::GetSelections().back() == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow : ImGuiTreeNodeFlags_DefaultOpen;
		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked() || ImGui::IsItemFocused())
		{
			if (EntitySelection::GetSelections().empty() || EntitySelection::GetSelections().back() != entity)
			{
				ViewportSelectionChangedEvent selectionEvent(entity, entity.GetUUID(), true);
				Application::Get().GetEditorLayer()->OnEvent(selectionEvent);
			}

			if(!EntitySelection::IsMultiSelect())
			EntitySelection::DeselectAll();

			EntitySelection::SelectEntity(entity);
			Application::Get().GetEditorLayer()->GetPropertiesPanel()->SetSelectedEntity(entity);
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			//TODO: Repurpose this when implementing group tagging

			//ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
			//bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			//if (opened)
			//	ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			sceneContext->DestroyEntity(entity);
			if (EntitySelection::IsSelected(entity))
			{
				EntitySelection::DeselectEntity(entity);
				Application::Get().GetEditorLayer()->GetPropertiesPanel()->SetSelectedEntity({});
				glm::vec3 trans = entity.GetComponent<TransformComponent>().Translation;
				ScreenReaderLogger::QueueOutput("Deselected " + entity.GetName() + " at X " + std::to_string((int)trans.x) + ", Y " + std::to_string((int)trans.y), true, true);
			}
		}
	}

	bool SceneHierarchyPanel::OnKeyPressed(KeyPressedEvent& keyEvent)
	{
		if (keyEvent.GetKeyCode() == Key::C)
			openEntityCreationWindow = !openEntityCreationWindow;

		if (keyEvent.GetKeyCode() == Key::LeftShift)
			openEntityCreationWindow = false;

		if (keyEvent.GetKeyCode() == Key::C || keyEvent.GetKeyCode() == Key::LeftShift)
		{
			if (openEntityCreationWindow)
				ScreenReaderLogger::QueueOutput("Opened entity creation window");
			else
				ScreenReaderLogger::QueueOutput("Closed entity creation window");
		}

		return false;
	}
}