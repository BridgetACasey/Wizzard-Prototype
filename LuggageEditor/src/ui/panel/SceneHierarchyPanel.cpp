//@BridgetACasey

#include "SceneHierarchyPanel.h"

#include <imgui/imgui.h>
#include "wizzard/scene/component/TagComponent.h"

namespace Wizzard
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		m_Context->registry.each([&](auto entityID)
		{
			Entity entity{ entityID , m_Context.get() };
		DrawEntityNode(entity);
		});

		//if (m_SquareEntity)
		//{
		//	ImGui::Separator();
		//	auto& tag = m_SquareEntity.GetComponent<TagComponent>().tag;
		//	ImGui::Text("%s", tag.c_str());
		//
		//	auto& squareColor = m_SquareEntity.GetComponent<SpriteComponent>().color;
		//	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
		//	ImGui::Separator();
		//}
		//
		//ImGui::DragFloat3("Camera Transform", glm::value_ptr(m_CameraEntity.GetComponent<TransformComponent>().GetTransform()[3]));
		//
		//if (ImGui::Checkbox("Camera A", &m_PrimaryCamera))
		//{
		//	m_CameraEntity.GetComponent<CameraComponent>().Primary = m_PrimaryCamera;
		//	m_SecondCamera.GetComponent<CameraComponent>().Primary = !m_PrimaryCamera;
		//}
		//
		//{
		//	auto& camera = m_SecondCamera.GetComponent<CameraComponent>().Camera;
		//	float orthoSize = camera.GetOrthographicSize();
		//	if (ImGui::DragFloat("Second Camera Ortho Size", &orthoSize))
		//		camera.SetOrthographicSize(orthoSize);
		//}
		//if (openConsoleMenu)
		//{
		//	auto stats = Renderer2D::GetStatistics();
		//	ImGui::Text("Renderer2D Stats:");
		//	ImGui::Text("Draw Calls: %d", stats.drawCalls);
		//	ImGui::Text("Quads: %d", stats.quadCount);
		//	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		//	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		//}

		ImGui::End();
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectionContext = entity;
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
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