//@BridgetACasey

#include <Wizzard.h>

class Application : public Wizzard::Application
{
public:
	Application()
	{

	}

	~Application()
	{

	}
};

Wizzard::Application* Application::createApplication()
{
	return new Application();
}