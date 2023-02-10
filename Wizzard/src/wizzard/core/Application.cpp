//@BridgetACasey

#include "wzpch.h"

#include <glfw/glfw3.h>

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
			float time = (float)glfwGetTime();
			Timestep timestep = time - lastFrameTime;
			lastFrameTime = time;

			for (Layer* layer : layerStack)
			{
				layer->OnUpdate(timestep);
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
