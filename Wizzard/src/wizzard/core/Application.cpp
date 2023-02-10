//@BridgetACasey

#include "wzpch.h"

#include "Application.h"
#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/event/EventHandler.h"

#include "wizzard/rendering/Renderer.h"

#include "Tolk.h"

#include "physics/Physics2D.h"

namespace Wizzard
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::appInstance = nullptr;

	Application::Application() : camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		WIZ_ASSERT(!appInstance, "Application already exists!");
		appInstance = this;

		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		imguiLayer = new ImGuiLayer();
		PushOverlay(imguiLayer);

		/////
		
		vertexArray.reset(VertexArray::Create());

		float vertices[3 * 7] =
		{
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

		std::shared_ptr<VertexBuffer> vertexBuff;
		vertexBuff.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float4, "a_Color" }
		};
		vertexBuff->SetLayout(layout);
		vertexArray->AddVertexBuffer(vertexBuff);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<IndexBuffer> indexBuff;
		indexBuff.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		vertexArray->SetIndexBuffer(indexBuff);

		squareVA.reset(VertexArray::Create());

		float squareVertices[3 * 4] =
		{
			-0.75f, -0.75f, 0.0f,
			 0.75f, -0.75f, 0.0f,
			 0.75f,  0.75f, 0.0f,
			-0.75f,  0.75f, 0.0f
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ ShaderDataType::Float3, "a_Position" }
			});
		squareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
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

		shader.reset(new Shader(vertexSrc, fragmentSrc));

		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
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

		blueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		// Tolk will also initialise COM if it has not been initialized on the calling thread
		Tolk_Load();

		if(Tolk_IsLoaded())
		{
			WIZ_INFO("Initialised Tolk Screen Reader Abstraction Library...");

			const wchar_t* screenreaderName = Tolk_DetectScreenReader();

			if (screenreaderName)
			{
				//Converting from wchar_t* to a format supported by spdlog, such as std::string
				std::wstring NameWStr(screenreaderName);
				std::string nameStr(NameWStr.begin(), NameWStr.end());

				WIZ_INFO("The current active screen reader driver is: {0}", nameStr);
			}
			else
				WIZ_WARN("None of the supported screen readers were detected!");

			if (Tolk_HasSpeech())
				WIZ_INFO("This screen reader driver supports speech");

			if (Tolk_HasBraille())
				WIZ_INFO("This screen reader driver supports braille");
		}
		else
			WIZ_WARN("Failed to initialise Tolk Screen Reader Abstraction Library!");

		while (running)
		{
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			camera.SetPosition({ 0.5f, 0.5f, 0.0f });
			camera.SetRotation(45.0f);

			Renderer::BeginScene(camera);

			Renderer::Submit(blueShader, squareVA);
			Renderer::Submit(shader, vertexArray);

			Renderer::EndScene();

			for (Layer* layer : layerStack)
			{
				layer->OnUpdate();
			}

			//for (Layer* layer : layerStack)
				imguiLayer->OnImGuiRender();

			window->OnUpdate();
		}

		WIZ_INFO("Unloading Tolk Screen Reader Abstraction Library...");
		Tolk_Unload();
	}

	void Application::OnEvent(Event& event)
	{
		EventHandler eventHandler(event);
		
		eventHandler.HandleEvent<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

		for (auto it = layerStack.end(); it != layerStack.begin();)
		{
			(*--it)->OnEvent(event);
		
			if (event.isHandled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		layerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		layerStack.PushOverlay(overlay);
	}

	bool Application::OnWindowClose(WindowCloseEvent& windowCloseEvent)
	{
		running = false;
		return true;
	}
}
