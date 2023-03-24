//@BridgetACasey

#include <wzpch.h>

#include "SceneHierarchyPanel.h"

#include <imgui.h>

#include "imgui_internal.h"
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
	}
	void SceneHierarchyPanel::OnImGuiRender()
	{
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

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				//selectionContext = {};
				EntitySelection::DeselectAll();

			static bool openCreatePopup = false;

			// Right-click on blank space
			//TODO: Fix later. Probably use event callbacks instead and pass to editor layer to handle.
			//if (Input::IsKeyPressed(Key::Enter) || Input::IsMouseButtonPressed(Mouse::RightButton))
				

			if(ImGuiSR::Button("CREATE ENTITY", ImVec2(440.0f, 80.5f)))
			{
				openCreatePopup = !openCreatePopup;
			}

			if (openCreatePopup)
			{
				if (ImGui::BeginPopup(0))
				{
					ImGui::SetKeyboardFocusHere();

					if (ImGui::MenuItem("Create Empty Entity"))
						sceneContext->CreateEntity("Empty Entity");

					ImGui::EndPopup();
				}
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

				//selectionData.eventCallback(selectionEvent);
			}

			//selectionContext = entity;
			if(!EntitySelection::IsMultiSelect())
			EntitySelection::DeselectAll();

			EntitySelection::SelectEntity(entity);
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
				//selectionContext = {};
				EntitySelection::DeselectEntity(entity);
		}
	}

	bool SceneHierarchyPanel::OnViewportSelectionChanged(ViewportSelectionChangedEvent& sceneEvent)
	{
		WIZ_TRACE(sceneEvent);

		return false;
	}
}