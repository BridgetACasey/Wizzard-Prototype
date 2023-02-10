//@BridgetACasey

#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "wizzard/rendering/Shader.h"
#include "wizzard/event/Event.h"
#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/imgui/ImGuiLayer.h"
#include "wizzard/rendering/Buffer.h"
#include "wizzard/rendering/VertexArray.h"
#include "wizzard/rendering/OrthographicCamera.h"

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

		std::shared_ptr<VertexArray> vertexArray;
		std::unique_ptr<VertexBuffer> vertexBuffer;
		std::unique_ptr<IndexBuffer> indexBuffer;
		std::shared_ptr<Shader> shader;
		std::shared_ptr<Shader> blueShader;
		std::shared_ptr<VertexArray> squareVA;

		OrthographicCamera camera;
	};

	Application* CreateApplication();	//To be defined in the client
}