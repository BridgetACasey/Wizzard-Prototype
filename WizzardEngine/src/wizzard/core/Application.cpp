//@BridgetACasey

#include "wzpch.h"

#include <glfw/glfw3.h>

#include "Application.h"

#include "ui/ImGuiScreenReading.h"
#include "wizzard/input/Input.h"
#include "wizzard/screenreading/ScreenReaderSupport.h"
#include "wizzard/audio/Audio.h"
#include "wizzard/rendering/GraphicsContext.h"
#include "wizzard/rendering/Renderer.h"

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
			float time = (float)glfwGetTime();	//TODO: Make this platform agnostic
			TimeStep timeStep = time - lastFrameTime;
			lastFrameTime = time;

			if (!minimised)
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
			graphicsContext->SwapBuffers();	//TODO: Find a better home for this instead of the run function
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

		for (auto it = layerStack.end(); it != layerStack.begin();)
		{
			if (event.isHandled)
				break;

			(*--it)->OnEvent(event);
		}
	}

	void Application::OnApplicationInit()
	{
		WIZ_PROFILE_FUNCTION();

		WIZ_TRACE("Initialising WIZZARD Engine application...");

		graphicsContext = GraphicsContext::Create();
		graphicsContext->Init();

		window->SetVSync(true);
		window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));

		Input::Init();
		Audio::Init();
		ScreenReaderSupport::Init();
		ImGuiSR::Init();
		Renderer::Init();

		WIZ_TRACE("WIZZARD Engine successfully initialised.");
	}

	void Application::OnApplicationShutdown()
	{
		WIZ_PROFILE_FUNCTION();

		WIZ_TRACE("Beginning WIZZARD Engine shutdown sequence...");

		Input::Shutdown();
		ScreenReaderSupport::Shutdown();
		ImGuiSR::Shutdown();
		Audio::Shutdown();
		Renderer::Shutdown();

		WIZ_TRACE("WIZZARD Engine successfully shutdown.");
	}

	void Application::PushLayer(Layer* layer)
	{
		WIZ_PROFILE_FUNCTION();

		WIZ_TRACE("Initialising layer '{0}' onto layer stack", layer->GetName());

		layerStack.PushLayer(layer);
	}

	void Application::PopLayer(Layer* layer)
	{
		WIZ_PROFILE_FUNCTION();

		WIZ_TRACE("Removing active layer '{0}' from layer stack", layer->GetName());

		layerStack.PopLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		WIZ_PROFILE_FUNCTION();

		WIZ_TRACE("Initialising overlay '{0}' onto layer stack", overlay->GetName());

		layerStack.PushOverlay(overlay);
	}

	void Application::PopOverlay(Layer* overlay)
	{
		WIZ_PROFILE_FUNCTION();

		WIZ_TRACE("Removing active overlay '{0}' from layer stack", overlay->GetName());

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
			minimised = true;
			return false;
		}

		minimised = false;
		Renderer::OnWindowResize(windowResizeEvent.GetWidth(), windowResizeEvent.GetHeight());

		return false;
	}
}