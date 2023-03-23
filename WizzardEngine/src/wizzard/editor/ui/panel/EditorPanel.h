//@BridgetACasey

#pragma once

#include "wizzard/base/Base.h"
#include "wizzard/scene/Scene.h"

namespace Wizzard
{
	class EditorPanel : public WizRefCounter
	{
	public:
		EditorPanel() = default;

		virtual void SetSceneContext(const WizRef<Scene>& scene);
		virtual void OnEvent(Event& event);
		virtual void OnImGuiRender();

	protected:
		WizRef<Scene> sceneContext;
	};
}