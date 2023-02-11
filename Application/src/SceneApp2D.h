//@BridgetACasey

#pragma once

#include "Wizzard.h"

class SceneApp2D : public Wizzard::Layer
{
public:
	SceneApp2D();
	virtual ~SceneApp2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Wizzard::Timestep timeStep) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Wizzard::Event& event) override;

private:
	Wizzard::OrthographicCameraController orthoCamController;

	// Temp
	Wizzard::Ref<Wizzard::VertexArray> squareVA;
	Wizzard::Ref<Wizzard::Shader> flatShader;

	Wizzard::Ref<Wizzard::Texture2D> lizardTexture;

	glm::vec4 squareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};