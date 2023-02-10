//@BridgetACasey

#include <Wizzard.h>

#include "imgui/imgui.h"

//#include "box2d/box2d.h"
#include "wizzard/physics/Physics2D.h"

class ExampleLayer : public Wizzard::Layer
{
public:
	ExampleLayer() : Layer("Example"), camera(-1.6f, 1.6f, -0.9f, 0.9f), cameraPosition(0.0f)
	{
		/////

		vertexArray.reset(Wizzard::VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Wizzard::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Wizzard::VertexBuffer::Create(vertices, sizeof(vertices)));

		std::shared_ptr<Wizzard::VertexBuffer> vertexBuff;
		vertexBuff.reset(Wizzard::VertexBuffer::Create(vertices, sizeof(vertices)));
		Wizzard::BufferLayout layout =
		{
			{Wizzard::ShaderDataType::Float3, "a_Position" },
			{Wizzard::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuff->SetLayout(layout);
		vertexArray->AddVertexBuffer(vertexBuff);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Wizzard::IndexBuffer> indexBuff;
		indexBuff.reset(Wizzard::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		vertexArray->SetIndexBuffer(indexBuff);

		squareVA.reset(Wizzard::VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<Wizzard::VertexBuffer> squareVB;
		squareVB.reset(Wizzard::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{Wizzard::ShaderDataType::Float3, "a_Position" }
			});
		squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Wizzard::IndexBuffer> squareIB;
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

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	
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

		shader.reset(new Wizzard::Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		blueShader.reset(new Wizzard::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	void OnUpdate(Wizzard::Timestep timeStep) override
	{
		if (Wizzard::Input::IsKeyPressed(Wizzard::Key::Left))
			cameraPosition.x -= cameraMoveSpeed * timeStep;
		else if (Wizzard::Input::IsKeyPressed(Wizzard::Key::Right))
			cameraPosition.x += cameraMoveSpeed * timeStep;

		if (Wizzard::Input::IsKeyPressed(Wizzard::Key::Up))
			cameraPosition.y += cameraMoveSpeed * timeStep;
		else if (Wizzard::Input::IsKeyPressed(Wizzard::Key::Down))
			cameraPosition.y -= cameraMoveSpeed * timeStep;

		if (Wizzard::Input::IsKeyPressed(Wizzard::Key::A))
			cameraRotation += cameraRotationSpeed * timeStep;
		if (Wizzard::Input::IsKeyPressed(Wizzard::Key::D))
			cameraRotation -= cameraRotationSpeed * timeStep;

		Wizzard::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Wizzard::RenderCommand::Clear();

		camera.SetPosition(cameraPosition);
		camera.SetRotation(cameraRotation);

		Wizzard::Renderer::BeginScene(camera);

		Wizzard::Renderer::Submit(blueShader, squareVA);
		Wizzard::Renderer::Submit(shader, vertexArray);

		Wizzard::Renderer::EndScene();
	}

	void OnImGuiRender() override
	{
		//ImGui::Begin("Test");
		//ImGui::Text("Hello World");
		//ImGui::End();
	}

	void OnEvent(Wizzard::Event& event) override
	{
		//APP_TRACE("{0}", event);
	}

private:
	std::shared_ptr<Wizzard::Shader> shader;
	std::shared_ptr<Wizzard::VertexArray> vertexArray;

	std::shared_ptr<Wizzard::Shader> blueShader;
	std::shared_ptr<Wizzard::VertexArray> squareVA;

	Wizzard::OrthographicCamera camera;
	glm::vec3 cameraPosition;
	float cameraMoveSpeed = 5.0f;

	float cameraRotation = 0.0f;
	float cameraRotationSpeed = 180.0f;
};

class UserApplication : public Wizzard::Application
{
public:
	UserApplication()
	{
		PushLayer(new ExampleLayer());
		//PushOverlay(new Wizzard::ImGuiLayer());
	}

	~UserApplication()
	{

	}
};

Wizzard::Application* Wizzard::CreateApplication()
{
	return new UserApplication();
}