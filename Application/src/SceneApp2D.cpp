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
}

void SceneApp2D::OnDetach()
{
}

void SceneApp2D::OnUpdate(Wizzard::TimeStep timeStep)
{
	orthoCamController.OnUpdate(timeStep);

	// Render
	Wizzard::Renderer2D::ResetStatistics();

	Wizzard::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Wizzard::RenderCommand::Clear();

	Wizzard::Renderer2D::BeginScene(orthoCamController.GetCamera());

	//Render objects

	Wizzard::Renderer2D::EndScene();
}

void SceneApp2D::OnImGuiRender()
{
}

void SceneApp2D::OnEvent(Wizzard::Event& event)
{
	orthoCamController.OnEvent(event);
}