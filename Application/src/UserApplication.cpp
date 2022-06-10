//@BridgetACasey

#include <Wizzard.h>

class ExampleLayer : public Wizzard::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{

	}

	void onUpdate() override
	{
		APP_INFO("ExampleLayer::Update");
	}

	void onEvent(Wizzard::Event& event) override
	{
		APP_TRACE("{0}", event);
	}
};

class UserApplication : public Wizzard::Application
{
public:
	UserApplication()
	{
		pushLayer(new ExampleLayer());
	}

	~UserApplication()
	{

	}
};

Wizzard::Application* Wizzard::createApplication()
{
	return new UserApplication();
}