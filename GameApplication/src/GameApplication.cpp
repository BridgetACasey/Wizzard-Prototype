//@BridgetACasey

#include <Wizzard.h>
#include <wizzard/base/EntryPoint.h>

#include "SceneApp2D.h"

class GameApplication : public Wizzard::Application
{
public:
	GameApplication()
	{
		editorLayer = new Wizzard::EditorLayer();
		PushLayer(editorLayer);

		//sceneApp = new SceneApp2D();
		//PushLayer(sceneApp);
	}

	~GameApplication()
	{
		//PopLayer(sceneApp);
		PopLayer(editorLayer);
	}

private:
	Wizzard::EditorLayer* editorLayer = nullptr;
	//SceneApp2D* sceneApp = nullptr;
};

Wizzard::Application* Wizzard::CreateApplication()
{
	return new GameApplication();
}