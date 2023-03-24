//@BridgetACasey

#pragma once

#include "wizzard/base/Base.h"
#include "Window.h"
#include "LayerStack.h"
#include "wizzard/event/Event.h"
#include "wizzard/event/EventHandler.h"
#include "wizzard/event/MulticastDelegate.h"
#include "wizzard/event/AudioEvent.h"
#include "wizzard/event/ApplicationEvent.h"
#include "wizzard/editor/EditorLayer.h"
#include "wizzard/editor/ui/imgui/ImGuiLayer.h"
#include "wizzard/base/Timestep.h"

int main(int argc, int** argv);

namespace Wizzard
{
	class GraphicsContext;

	class Application
	{
	public:
		Application(const std::string& name = "Wizzard Application");
		virtual ~Application();

		void Close();

		void OnEvent(Event& event);

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		bool HasTriggeredShutdown() const { return triggeredShutdown; }
		MulticastDelegate<AppShutdownEvent>& GetAppShutdownDelegate() { return appShutdownDelegate; }

		ImGuiLayer* GetImGuiLayer() { return imguiLayer; }
		EditorLayer* GetEditorLayer() { return editorLayer; }

		Window& GetWindow() { return *window; }

		static Application& Get() { return *appInstance; }

	protected:
		virtual void OnApplicationInit();
		virtual void OnApplicationShutdown();

	private:
		void Run();
		friend int::main(int argc, int** argv);

		bool OnWindowClose(WindowCloseEvent& windowCloseEvent);
		bool OnWindowResize(WindowResizeEvent& windowResizeEvent);

		std::unique_ptr<Window> window;

		GraphicsContext* graphicsContext;

		bool running = true;
		bool triggeredShutdown = false;
		bool minimised = false;

		LayerStack layerStack;

		static Application* appInstance;

		ImGuiLayer* imguiLayer;
		EditorLayer* editorLayer;

		float lastFrameTime = 0.0f;

		MulticastDelegate<AppShutdownEvent> appShutdownDelegate;

		friend class ApplicationSettingsPanel;
	};

	Application* CreateApplication();	//To be defined in the client
}