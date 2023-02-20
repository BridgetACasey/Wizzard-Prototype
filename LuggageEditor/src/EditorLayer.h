//@BridgetACasey

#pragma once

#include "Wizzard.h"

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
	};
}