#pragma once

#include "Wizzard.h"

namespace Wizzard
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		void OnUpdate(Timestep timeStep) override;
		virtual void OnImGuiRender() override;
		void OnEvent(Event& event) override;

	private:
		Wizzard::OrthographicCameraController orthoCamController;

		// Temp
		Ref<VertexArray> squareVA;
		Ref<Shader> flatShader;
		Ref<Framebuffer> frameBuffer;

		Ref<Texture2D> customTexture;

		AudioSource music;
		bool playMusic = false;

		glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
	};
}