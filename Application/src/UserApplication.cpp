//@BridgetACasey

#include <Wizzard.h>
#include <wizzard/base/EntryPoint.h>

#include "imgui/imgui.h"

#include "SceneApp2D.h"

class UserApplication : public Wizzard::Application
{
public:
	UserApplication()
	{
		PushLayer(new SceneApp2D());
	}

	~UserApplication()
	{

	}
};

Wizzard::Application* Wizzard::CreateApplication()
{
	return new UserApplication();
}