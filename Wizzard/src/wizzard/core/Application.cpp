//@BridgetACasey

#include "wzpch.h"

#include "Application.h"
#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/event/EventHandler.h"

#include <glad/glad.h>

#include "Tolk.h"

using namespace std;

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

		float vertices[3 * 3] = {
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

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
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
		wcout << L"Tolk -- C++ Console App Example" << endl;

		wcout << L"Initializing Tolk..." << endl;
		// Tolk will also initialize COM
		// if it has not been initialized on the calling thread
		Tolk_Load();

		wcout << L"Querying for the active screen reader driver..." << endl;
		const wchar_t* name = Tolk_DetectScreenReader();
		if (name) {
			wcout << L"The active screen reader driver is: " << name << endl;
		}
		else {
			wcout << L"None of the supported screen readers is running" << endl;
		}

		if (Tolk_HasSpeech()) {
			wcout << L"This screen reader driver supports speech" << endl;
		}
		if (Tolk_HasBraille()) {
			wcout << L"This screen reader driver supports braille" << endl;
		}

		wcout << L"Let's output some text..." << endl;
		if (!Tolk_Output(L"Hello, World!")) {
			wcout << L"Failed to output text" << endl;
		}

		wcout << L"Finalizing Tolk..." << endl;
		// Tolk will also try to uninitialize COM
		Tolk_Unload();

		wcout << L"Done!" << endl;

		while (running)
		{
			glClearColor(0.5, 0.5, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			shader->Bind();
			glBindVertexArray(vertexArray);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

			for (Layer* layer : layerStack)
			{
				layer->OnUpdate();
			}

			//for (Layer* layer : layerStack)
				imguiLayer->OnImGuiRender();

			window->OnUpdate();
		}
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
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& windowCloseEvent)
	{
		running = false;
		return true;
	}
}
