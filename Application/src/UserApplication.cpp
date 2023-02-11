//@BridgetACasey

#include <Wizzard.h>
#include <wizzard/core/EntryPoint.h>

#include "imgui/imgui.h"

//#include "box2d/box2d.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "wizzard/physics/Physics2D.h"

#include "wizzard/platform/opengl/OpenGLShader.h"
#include "wizzard/rendering/Texture.h"
#include "wizzard/core/OrthographicCameraController.h"

#include "SceneApp2D.h"

class ExampleLayer : public Wizzard::Layer
{
public:
	ExampleLayer() : Layer("Example"), orthoCamController(1920.0f / 1080.0f)
	{
		vertexArray = Wizzard::VertexArray::Create();

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Wizzard::Ref<Wizzard::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Wizzard::VertexBuffer::Create(vertices, sizeof(vertices)));

		Wizzard::Ref<Wizzard::VertexBuffer> vertexBuff;
		vertexBuff.reset(Wizzard::VertexBuffer::Create(vertices, sizeof(vertices)));
		Wizzard::BufferLayout layout =
		{
			{Wizzard::ShaderDataType::Float3, "a_Position" },
			{Wizzard::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuff->SetLayout(layout);
		vertexArray->AddVertexBuffer(vertexBuff);

		uint32_t indices[3] = { 0, 1, 2 };
		Wizzard::Ref<Wizzard::IndexBuffer> indexBuff;
		indexBuff.reset(Wizzard::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		vertexArray->SetIndexBuffer(indexBuff);

		squareVA = Wizzard::VertexArray::Create();

		float squareVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Wizzard::Ref<Wizzard::VertexBuffer> squareVB;
		squareVB.reset(Wizzard::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{Wizzard::ShaderDataType::Float3, "a_Position"},
			{Wizzard::ShaderDataType::Float2, "a_TexCoord"}
			});
		squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Wizzard::Ref<Wizzard::IndexBuffer> squareIB;
		squareIB.reset(Wizzard::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		squareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc =
			R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;
			out vec4 v_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc =
			R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		shader = Wizzard::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string flatFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		flatShader = Wizzard::Shader::Create("FlatColor", flatVertexSrc, flatFragmentSrc);

		auto texShader = shaderLibrary.Load("res/shaders/TextureShader.glsl");

		texture = Wizzard::Texture2D::Create("res/textures/potatolizard.png");
		logoTexture = Wizzard::Texture2D::Create("res/textures/eso-launch-all-icon.png");

		std::dynamic_pointer_cast<Wizzard::OpenGLShader>(texShader)->Bind();
		std::dynamic_pointer_cast<Wizzard::OpenGLShader>(texShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Wizzard::Timestep timeStep) override
	{
		orthoCamController.OnUpdate(timeStep);

		Wizzard::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Wizzard::RenderCommand::Clear();

		Wizzard::Renderer::BeginScene(orthoCamController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Wizzard::OpenGLShader>(flatShader)->Bind();
		std::dynamic_pointer_cast<Wizzard::OpenGLShader>(flatShader)->UploadUniformFloat3("u_Color", squareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Wizzard::Renderer::Submit(flatShader, squareVA, transform);
			}
		}

		auto texShader = shaderLibrary.Get("TextureShader");

		texture->Bind();
		Wizzard::Renderer::Submit(texShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		logoTexture->Bind();
		Wizzard::Renderer::Submit(texShader, squareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Wizzard::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(squareColor));
		ImGui::End();
	}

	void OnEvent(Wizzard::Event& event) override
	{
		orthoCamController.OnEvent(event);
	}

private:
	Wizzard::Ref<Wizzard::Shader> shader;
	Wizzard::ShaderLibrary shaderLibrary;
	Wizzard::Ref<Wizzard::VertexArray> vertexArray;

	Wizzard::Ref<Wizzard::Shader> flatShader;
	Wizzard::Ref<Wizzard::VertexArray> squareVA;
	Wizzard::Ref<Wizzard::Texture2D> texture, logoTexture;

	Wizzard::OrthographicCameraController orthoCamController;

	glm::vec3 squareColor = { 0.2f, 0.3f, 0.8f };
};

class UserApplication : public Wizzard::Application
{
public:
	UserApplication()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new SceneApp2D());
	}

	~UserApplication()
	{

	}
};

Wizzard::Application* Wizzard::CreateApplication()
{
	return new UserApplication();
}