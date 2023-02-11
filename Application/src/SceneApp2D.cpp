//@BridgetACasey

#include "SceneApp2D.h"

#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "imgui/imgui.h"
#include "wizzard/platform/opengl/OpenGLShader.h"

SceneApp2D::SceneApp2D() : Layer("Sandbox2D"), orthoCamController(1920.0f / 1080.0f)
{
}

void SceneApp2D::OnAttach()
{
	squareVA = Wizzard::VertexArray::Create();

	float squareVertices[5 * 4] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	Wizzard::Ref<Wizzard::VertexBuffer> squareVB;
	squareVB.reset(Wizzard::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));

	squareVB->SetLayout({
		{ Wizzard::ShaderDataType::Float3, "a_Position" }
		});
	squareVA->AddVertexBuffer(squareVB);

	uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	Wizzard::Ref<Wizzard::IndexBuffer> squareIB;
	squareIB.reset(Wizzard::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	squareVA->SetIndexBuffer(squareIB);

	flatShader = Wizzard::Shader::Create("res/shaders/FlatColorShader.glsl");
}

void SceneApp2D::OnDetach()
{
}

void SceneApp2D::OnUpdate(Wizzard::Timestep timeStep)
{
	orthoCamController.OnUpdate(timeStep);

	Wizzard::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Wizzard::RenderCommand::Clear();

	Wizzard::Renderer::BeginScene(orthoCamController.GetCamera());

	std::dynamic_pointer_cast<Wizzard::OpenGLShader>(flatShader)->Bind();
	std::dynamic_pointer_cast<Wizzard::OpenGLShader>(flatShader)->UploadUniformFloat4("u_Color", squareColor);

	Wizzard::Renderer::Submit(flatShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

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