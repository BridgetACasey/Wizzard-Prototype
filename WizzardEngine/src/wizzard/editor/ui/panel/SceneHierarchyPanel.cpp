//@BridgetACasey

#include <wzpch.h>

#include "SceneHierarchyPanel.h"

#include <imgui.h>

#include "imgui_internal.h"
#include "wizzard/scene/component/TagComponent.h"
#include "wizzard/editor/ui/imgui/ImGuiScreenReading.h"

namespace Wizzard
{
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGuiSR::WindowBegin("Scene Hierarchy");

		if (sceneContext)
		{
			sceneContext->registry.each([&](auto entityID)
			{
				Entity entity{ entityID , sceneContext.get() };
				DrawEntityNode(entity);
			});

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
				selectionContext = {};

			// Right-click on blank space
			if (ImGui::BeginPopupContextWindow(0))
			{
				if (ImGui::MenuItem("Create Empty Entity"))
					sceneContext->CreateEntity("Empty Entity");

				ImGui::EndPopup();
			}

		}
		ImGui::End();

		ImGuiSR::WindowBegin("Properties");
		if (selectionContext)
		{
			DrawComponents(selectionContext);
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		selectionContext = entity;
	}

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		//auto& tag = entity.GetComponent<TagComponent>().tag;
		//
		//ImGuiTreeNodeFlags flags = ((selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		//flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
		//bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		//if (ImGui::IsItemClicked())
		//{
		//	selectionContext = entity;
		//}
		//
		//bool entityDeleted = false;
		//if (ImGui::BeginPopupContextItem())
		//{
		//	if (ImGui::MenuItem("Delete Entity"))
		//		entityDeleted = true;
		//
		//	ImGui::EndPopup();
		//}
		//
		//if (opened)
		//{
		//	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		//	bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
		//	if (opened)
		//		ImGui::TreePop();
		//	ImGui::TreePop();
		//}
		//
		//if (entityDeleted)
		//{
		//	sceneContext->DestroyEntity(entity);
		//	if (selectionContext == entity)
		//		selectionContext = {};
		//}
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			selectionContext = entity;
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}
	}
}