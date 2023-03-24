//@BridgetACasey

#include <Wizzard.h>
#include <wizzard/base/EntryPoint.h>

#include "GameApplicationLayer.h"

class GameApplication : public Wizzard::Application
{
public:
	GameApplication()
	{
		sceneApp = new GameApplicationLayer();
		PushLayer(sceneApp);
	}

	~GameApplication()
	{
		PopLayer(sceneApp);
	}

private:
	GameApplicationLayer* sceneApp = nullptr;
};

Wizzard::Application* Wizzard::CreateApplication()
{
	return new GameApplication();
}