//@BridgetACasey

#include <Wizzard.h>

#include "imgui/imgui.h"

//#include "box2d/box2d.h"
#include "wizzard/physics/Physics2D.h"

class ExampleLayer : public Wizzard::Layer
{
public:
	ExampleLayer() : Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//APP_INFO("ExampleLayer::Update");
		//if (Wizzard::Input::IsKeyPressed(Wizzard::Key::Tab))
		//	WIZ_TRACE("Tab key is pressed (poll)!");

		static bool test = true;

		if (test)
		{
			Wizzard::Physics2D physics;
			physics.CreateBox2DWorld();
			APP_INFO("Physic time weeeeee: {0}", physics.gravity.y);
			test = false;
		}
	}

	virtual void OnImGuiRender() override
	{
		//ImGui::Begin("Test");
		//ImGui::Text("Hello World");
		//ImGui::End();
	}

	void OnEvent(Wizzard::Event& event) override
	{
		//APP_TRACE("{0}", event);
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