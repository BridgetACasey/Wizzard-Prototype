//@BridgetACasey

#include "SceneApp2D.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "wizzard/platform/opengl/OpenGLShader.h"

SceneApp2D::SceneApp2D() : Layer("SceneApp2D"), orthoCamController(1920.0f / 1080.0f)
{
}

void SceneApp2D::OnAttach()
{
	customTexture = Wizzard::Texture2D::Create("res/textures/kate-derp.png");
}

void SceneApp2D::OnDetach()
{
}

void SceneApp2D::OnUpdate(Wizzard::Timestep timeStep)
{
	orthoCamController.OnUpdate(timeStep);

	// Render
	Wizzard::Renderer2D::ResetStats();
	{
		Wizzard::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Wizzard::RenderCommand::Clear();
	}

	static float rotation = 0.0f;
	rotation += timeStep * 50.0f;

	Wizzard::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Wizzard::RenderCommand::Clear();

	Wizzard::Renderer2D::BeginScene(orthoCamController.GetCamera());
	Wizzard::Renderer2D::DrawRotatedQuad({ 1.0f, 0.0f }, { 0.8f, 0.8f }, -45.0f, { 0.8f, 0.2f, 0.3f, 1.0f });
	Wizzard::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Wizzard::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f }, { 0.2f, 0.3f, 0.8f, 1.0f });
	Wizzard::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, customTexture, 10.0f);
	Wizzard::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }, rotation, customTexture, 1.0f);
	Wizzard::Renderer2D::EndScene();

	Wizzard::Renderer::EndScene();
}

void SceneApp2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(squareColor));
	ImGui::End();
}

void SceneApp2D::OnEvent(Wizzard::Event& event)
{
	orthoCamController.OnEvent(event);
}