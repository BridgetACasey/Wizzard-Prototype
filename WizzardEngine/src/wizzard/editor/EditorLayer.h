//@BridgetACasey

#pragma once

#include "wizzard/audio/AudioSource.h"

#include "wizzard/common/Layer.h"
#include "wizzard/common/OrthographicCameraController.h"

#include "wizzard/rendering/Framebuffer.h"
#include "wizzard/rendering/Shader.h"
#include "wizzard/rendering/Texture.h"
#include "wizzard/rendering/VertexArray.h"
#include "wizzard/rendering/EditorCamera.h"

#include "wizzard/editor/ui/panel/ApplicationSettingsPanel.h"
#include "wizzard/editor/ui/panel/SceneHierarchyPanel.h"
#include "wizzard/editor/ui/panel/ObjectCreationPanel.h"
#include "wizzard/editor/ui/panel/ObjectPropertiesPanel.h"

namespace Wizzard
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void OnUpdate(TimeStep timeStep) override;
		void OnImGuiRender() override;
		void OnEvent(Event& event) override;

	private:
		OrthographicCameraController orthoCamController;
		EditorCamera m_EditorCamera;

		Ref<Scene> activeScene;
		Ref<Scene> editorScene;

		Entity m_SquareEntity;
		Entity m_CameraEntity;

		int m_GizmoType = -1;

		bool m_PrimaryCamera = true;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		glm::vec2 m_ViewportBounds[2];

		// Temp
		Ref<VertexArray> squareVA;
		Ref<Shader> flatShader;
		Ref<Framebuffer> frameBuffer;

		Ref<Texture2D> customTexture;

		AudioSource music;
		bool playMusic = false;

		bool isViewportFocused = false;
		bool isViewportHovered = false;

		glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		enum class SceneState
		{
			Edit = 0, Play = 1
		};
		SceneState m_SceneState = SceneState::Edit;

		ApplicationSettingsPanel appSettingsPanel;
		SceneHierarchyPanel sceneHierarchyPanel;
		ObjectCreationPanel objCreatePanel;
		ObjectPropertiesPanel objPropertiesPanel;

		Entity hoveredEntity;
	};
}