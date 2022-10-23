//@BridgetACasey

#include <Wizzard.h>

class ExampleLayer : public Wizzard::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}

	void OnUpdate() override
	{
		//APP_INFO("ExampleLayer::Update");
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
		PushOverlay(new Wizzard::ImGuiLayer());
	}

	~UserApplication()
	{

	}
};

Wizzard::Application* Wizzard::CreateApplication()
{
	return new UserApplication();
}