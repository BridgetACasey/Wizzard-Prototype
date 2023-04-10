//@BridgetACasey

#include <wzpch.h>

#include "PropertiesPanel.h"

#include "imgui_internal.h"
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

namespace Wizzard
{
	void PropertiesPanel::OnImGuiRender()
	{
		if (shouldTriggerFocus)
		{
			ImGui::SetNextWindowFocus();
			ScreenReaderLogger::QueueOutput("OBJECT PROPERTIES");
			Audio::Play(Audio::GetEditorAudioSource(WIZ_AUDIO_UIWINDOWCHANGED));
			shouldTriggerFocus = false;
		}

		ImGuiSR::Begin("PROPERTIES", nullptr, 0, "Object properties.", true);
		if (selectionContext)
		{
			DrawComponents(selectionContext);
		}
		ImGui::End();
	}

	template<typename T>
	void PropertiesPanel::DisplayAddComponentEntry(const std::string& entryName)
	{
		if (!selectionContext.HasComponent<T>())
		{
			if (ImGuiSR::MenuItem(entryName.c_str()))
			{
				selectionContext.AddComponent<T>();
				ImGui::CloseCurrentPopup();
			}
		}
	}

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGuiSR::Button("X", buttonSize, "Reset " + label + "X", true))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGuiSR::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f", 0, label + " X", true);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGuiSR::Button("Y", buttonSize, "Reset " + label + "Y", true))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGuiSR::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f", 0, label + " Y", true);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		if (ImGuiSR::Button("Z", buttonSize, "Reset " + label + "Z", true))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGuiSR::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f", 0, label + " Z", true);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	template<typename T, typename UIFunction>
	static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
	{
		const ImGuiTreeNodeFlags treeNodeFlags = /*ImGuiTreeNodeFlags_DefaultOpen |*/ ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		if (entity.HasComponent<T>())
		{
			auto& component = entity.GetComponent<T>();
			//ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			bool removeComponent = false;
			if (ImGui::BeginPopup("RemoveComponent"))
			{
				if (ImGuiSR::MenuItem("Remove " + name + " component"))
					removeComponent = true;

				ImGui::EndPopup();
				ImGui::SetNextItemOpen(false);
			}

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
			//float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			ImGui::Separator();
			bool open = ImGuiSR::TreeNodeEx(name.c_str(), (void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());

			ImGui::PopStyleVar();
			//ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			//if (ImGuiSR::Button("-", ImVec2{ lineHeight, lineHeight }, "Remove Component", true))
			//{
			//	ImGui::OpenPopup("ComponentSettings");
			//}

			//ImGui::TreeNodeSetOpen((void*)typeid(T).hash_code(), treeFocused);

			if (open)
			{
				uiFunction(component);
				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<T>();
		}
	}

	void PropertiesPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
			if (ImGuiSR::InputText("##Tag", buffer, sizeof(buffer), 0, 0, NULL, ("Rename " + tag), true))
			{
				tag = std::string(buffer);
			}

			if(ImGui::IsItemFocused()/* || ImGui::IsItemHovered()*/)
			{
				Input::SetQueryInput(false);
			}
			else
			{
				if (!Input::IsQueryingInput())
					Input::SetQueryInput(true);
			}
		}

		//TODO: Re-add at a later date
		ImGui::SameLine();
		ImGui::PushItemWidth(-1);

		float lineWidth = GImGui->Font->FontSize + GImGui->Style.FramePadding.x * 2.0f;
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

		if (ImGuiSR::Button("+", ImVec2{lineWidth, lineHeight}, "Add Component", true))
			ImGui::OpenPopup("AddComponent");

		ImGui::SameLine();
		if (ImGuiSR::Button("-", ImVec2{ lineWidth, lineHeight }, "Remove Component", true))
		{
			ImGui::OpenPopup("RemoveComponent");
		}

		if (ImGui::BeginPopup("AddComponent"))
		{
			DisplayAddComponentEntry<CameraComponent>("Camera");
			DisplayAddComponentEntry<SpriteComponent>("Sprite");
			DisplayAddComponentEntry<RigidBody2DComponent>("RigidBody 2D");
			DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
			DisplayAddComponentEntry<CharacterControllerComponent>("Character Controller");
			//DisplayAddComponentEntry<AudioListenerComponent>("Audio Listener");

			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();

		DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec3Control("Translate", component.Translation);
				glm::vec3 rotation = glm::degrees(component.Rotation);
				DrawVec3Control("Rotate", rotation);
				component.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", component.Scale, 1.0f);
			});

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				auto& camera = component.Camera;

				ImGuiSR::Checkbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGuiSR::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];
						if (ImGuiSR::Selectable(projectionTypeStrings[i], ImVec2{}, isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGuiSR::DragFloat("Vertical FOV", &perspectiveVerticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGuiSR::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGuiSR::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGuiSR::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGuiSR::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGuiSR::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGuiSR::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});

		DrawComponent<SpriteComponent>("Sprite", entity, [](auto& component)
			{
				ImGuiSR::ColorEdit4("Color", glm::value_ptr(component.color));
			});

		DrawComponent<RigidBody2DComponent>("RigidBody 2D", entity, [](auto& component)
			{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
				if (ImGuiSR::BeginCombo("Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGuiSR::Selectable(bodyTypeStrings[i], ImVec2{}, isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							component.Type = (RigidBody2DComponent::BodyType)i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				ImGuiSR::Checkbox("Fixed Rotation", &component.FixedRotation);
			});

		DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
			{
				ImGuiSR::DragFloat2("Offset", glm::value_ptr(component.Offset));
				ImGuiSR::DragFloat2("Size", glm::value_ptr(component.Size));
				ImGuiSR::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
				ImGuiSR::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
				ImGuiSR::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
				ImGuiSR::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
			});

		DrawComponent<CharacterControllerComponent>("Character Controller", entity, [](auto& component)
			{
				ImGuiSR::Checkbox("Disable Gravity", &component.disableGravity);
			});

		DrawComponent<AudioListenerComponent>("Audio Listener", entity, [](auto& component)
			{
				ImGuiSR::Checkbox("Active", &component.isActive);
			});
	}
}