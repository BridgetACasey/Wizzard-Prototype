//@BridgetACasey

#include <Wizzard.h>

class UserApplication : public Wizzard::Application
{
public:
	UserApplication()
	{

	}

	~UserApplication()
	{

	}
};

Wizzard::Application* Wizzard::createApplication()
{
	return new UserApplication();
}