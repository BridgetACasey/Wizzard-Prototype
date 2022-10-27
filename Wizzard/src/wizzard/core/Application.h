//@BridgetACasey

#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "wizzard/rendering/Shader.h"
#include "wizzard/event/Event.h"
#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/imgui/ImGuiLayer.h"

namespace Wizzard
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		Window& GetWindow() { return *window; }

		static Application& Get() { return *appInstance; }

	private:
		bool OnWindowClose(WindowCloseEvent& windowCloseEvent);
		
		std::unique_ptr<Window> window;

		bool running = true;

		LayerStack layerStack;

		static Application* appInstance;

		ImGuiLayer* imguiLayer;

		unsigned int vertexArray, vertexBuffer, indexBuffer;
		std::unique_ptr<Shader> shader;
	};

	Application* CreateApplication();	//To be defined in the client
}