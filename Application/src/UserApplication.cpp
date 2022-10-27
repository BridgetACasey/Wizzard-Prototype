//@BridgetACasey

#include <Wizzard.h>

#include "imgui/imgui.h"

class ExampleLayer : public Wizzard::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}

	void OnUpdate() override
	{
		//APP_INFO("ExampleLayer::Update");
		if (Wizzard::Input::IsKeyPressed(Wizzard::Key::Tab))
			WIZ_TRACE("Tab key is pressed (poll)!");
	}

	virtual void OnImGuiRender() override
	{
		//ImGui::Begin("Test");
		//ImGui::Text("Hello World");
		//ImGui::End();
	}

	void OnEvent(Wizzard::Event& event) override
	{
		APP_TRACE("{0}", event);
	}
};

class UserApplication : public Wizzard::Application
{
public:
	UserApplication()
	{
		PushLayer(new ExampleLayer());
		//PushOverlay(new Wizzard::ImGuiLayer());
	}

	~UserApplication()
	{

	}
};

Wizzard::Application* Wizzard::CreateApplication()
{
	return new UserApplication();
}