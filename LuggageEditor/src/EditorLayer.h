//@BridgetACasey

#pragma once

#include "Wizzard.h"
#include "wizzard/rendering/EditorCamera.h"
#include "ui/panel/SceneHierarchyPanel.h"

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
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		Entity m_SecondCamera;

		bool m_PrimaryCamera = true;

		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };

		// Temp
		Ref<VertexArray> squareVA;
		Ref<Shader> flatShader;
		Ref<Framebuffer> frameBuffer;

		Ref<Texture2D> customTexture;

		AudioSource music;
		bool playMusic = false;

		bool isViewportFocused = false;
		bool isViewportHovered = false;

		bool isMainMenuFocused = false;
		bool isMainMenuHovered = false;

		bool isHierarchyFocused = false;
		bool isHierarchyHovered = false;

		glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };

		SceneHierarchyPanel sceneHierarchyPanel;
	};
}