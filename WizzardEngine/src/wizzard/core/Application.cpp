//@BridgetACasey

#include "wzpch.h"

#include <glfw/glfw3.h>

#include "Application.h"
#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/event/UIEvent.h"
#include "wizzard/event/EventHandler.h"

#include "wizzard/input/Input.h"
#include "wizzard/screenreading/ScreenReaderSupport.h"
#include "wizzard/audio/Audio.h"
#include "wizzard/rendering/Renderer.h"

#include "Tolk.h"

#include "physics/Physics2D.h"

namespace Wizzard
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::appInstance = nullptr;

	Application::Application(const std::string& name)
	{
		WIZ_PROFILE_FUNCTION();

		WIZ_ASSERT(!appInstance, "Application already exists!");
		appInstance = this;

		window = std::unique_ptr<Window>(Window::Create(WindowProps(name)));
		window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		OnApplicationInit();

		imguiLayer = new ImGuiLayer();
		PushOverlay(imguiLayer);
	}

	Application::~Application()
	{
		WIZ_PROFILE_FUNCTION();

		OnApplicationShutdown();
	}

	void Application::Run()
	{
		WIZ_PROFILE_FUNCTION();

		while (running)
		{
			float time = (float)glfwGetTime();
			TimeStep timeStep = time - lastFrameTime;
			lastFrameTime = time;

			if (!minimized)
			{
				//Update all layers
				for (Layer* layer : layerStack)
				{
					layer->OnUpdate(timeStep);
				}

				//Render all layers' imgui elements
				imguiLayer->Begin();

				for(Layer* layer : layerStack)
				{
					layer->OnImGuiRender();
				}

				imguiLayer->End();
			}

			window->OnUpdate();
		}
	}

	void Application::Close()
	{
		WIZ_PROFILE_FUNCTION();

		running = false;
	}

	void Application::OnEvent(Event& event)
	{
		WIZ_PROFILE_FUNCTION();

		EventHandler eventHandler(event);
		
		eventHandler.HandleEvent<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
		eventHandler.HandleEvent<WindowResizeEvent>(BIND_EVENT_FN(Application::OnWindowResize));

		eventHandler.HandleEvent<UIElementFocusEvent>(BIND_EVENT_FN(Application::OnUIElementFocus));
		eventHandler.HandleEvent<UIElementSelectedEvent>(BIND_EVENT_FN(Application::OnUIElementSelected));

		for (auto it = layerStack.end(); it != layerStack.begin();)
		{
			(*--it)->OnEvent(event);
		
			if (event.isHandled)
				break;
		}
	}

	void Application::OnApplicationInit()
	{
		WIZ_PROFILE_FUNCTION();

		WIZ_INFO("Initialising WIZZARD Engine application...");

		Input::Init();
		ScreenReaderSupport::Init();
		Audio::Init();
		Renderer::Init();

		WIZ_INFO("WIZZARD Engine successfully initialised.");
	}

	void Application::OnApplicationShutdown()
	{
		WIZ_PROFILE_FUNCTION();

		WIZ_INFO("Beginning WIZZARD Engine shutdown sequence...");

		Input::Shutdown();
		ScreenReaderSupport::Shutdown();
		Audio::Shutdown();
		Renderer::Shutdown();

		WIZ_INFO("WIZZARD Engine successfully shutdown.");
	}

	void Application::PushLayer(Layer* layer)
	{
		WIZ_PROFILE_FUNCTION();

		layerStack.PushLayer(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		WIZ_PROFILE_FUNCTION();

		layerStack.PopLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		WIZ_PROFILE_FUNCTION();

		layerStack.PushOverlay(overlay);
	}

	void Application::PopOverlay(Layer* overlay)
	{
		WIZ_PROFILE_FUNCTION();

		layerStack.PopOverlay(overlay);
	}

	bool Application::OnWindowClose(WindowCloseEvent& windowCloseEvent)
	{
		WIZ_PROFILE_FUNCTION();

		running = false;

		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& windowResizeEvent)
	{
		WIZ_PROFILE_FUNCTION();

		if (windowResizeEvent.GetWidth() == 0 || windowResizeEvent.GetHeight() == 0)
		{
			minimized = true;
			return false;
		}

		minimized = false;
		Renderer::OnWindowResize(windowResizeEvent.GetWidth(), windowResizeEvent.GetHeight());

		return false;
	}

	bool Application::OnUIElementFocus(UIElementFocusEvent& uiElementFocusEvent)
	{
		WIZ_TRACE(uiElementFocusEvent);

		return false;
	}

	bool Application::OnUIElementSelected(UIElementSelectedEvent& uiElementSelectedEvent)
	{
		WIZ_TRACE(uiElementSelectedEvent);

		return false;
	}
}