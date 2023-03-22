//@BridgetACasey

#include "wzpch.h"

#include "EditorPanel.h"

namespace Wizzard
{
	void EditorPanel::SetSceneContext(const WizRef<Scene>& scene)
	{
		sceneContext = scene;
	}

	void EditorPanel::OnEvent(Event& event)
	{
	}

	void EditorPanel::OnImGuiRender()
	{
	}
}