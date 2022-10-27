//@BridgetACasey

#include "wzpch.h"

#include "Application.h"
#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/event/EventHandler.h"

#include <glad/glad.h>

#include "Input.h"

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
	}

	Application::~Application()
	{
		
	}

	void Application::Run()
	{
		while (running)
		{
			glClearColor(0.5, 0.5, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			for (Layer* layer : layerStack)
			{
				layer->OnUpdate();
			}

			//for (Layer* layer : layerStack)
				imguiLayer->OnImGuiRender();

			//Testing input polling
			//auto [x, y] = Input::GetMousePosition();
			//WIZ_TRACE("{0}, {1}", x, y);

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