//@BridgetACasey

#include "wzpch.h"

#include "Application.h"
#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/event/EventHandler.h"

#include <glad/glad.h>

#include "Tolk.h"

#include "physics/Physics2D.h"

namespace Wizzard
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::appInstance = nullptr;

	Application::Application()
	{
		WIZ_ASSERT(!appInstance, "Application already exists!");
		appInstance = this;

		window = std::unique_ptr<Window>(Window::Create());
		window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		imguiLayer = new ImGuiLayer();
		PushOverlay(imguiLayer);

		/////
		
		glGenVertexArrays(1, &vertexArray);
		glBindVertexArray(vertexArray);

		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		float vertices[3 * 3] =
		{
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.0f,  0.5f, 0.0f
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		unsigned int indices[3] = { 0, 1, 2 };
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		std::string vertexSrc =
		R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc =
		R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		shader.reset(new Shader(vertexSrc, fragmentSrc));
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

		//Physics2D physics;

		//physics.CreateBox2DWorld();

		//WIZ_INFO("Box2D connected to Wizzard, gravity value is: {0}", physics.gravity.y);

		while (running)
		{
			glClearColor(0.5, 0.5, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			//Commenting out triangle render for now while I work on screen reader support
			//shader->Bind();
			//glBindVertexArray(vertexArray);
			//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

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
