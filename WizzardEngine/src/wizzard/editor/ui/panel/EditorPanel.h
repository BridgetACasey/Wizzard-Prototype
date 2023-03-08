//@BridgetACasey

#pragma once

#include "wizzard/base/Base.h"
#include "wizzard/scene/Scene.h"

namespace Wizzard
{
	class EditorPanel
	{
	public:
		EditorPanel() = default;

		virtual void SetSceneContext(const Ref<Scene>& scene);
		virtual void OnEvent(Event& event);
		virtual void OnImGuiRender();

	protected:
		Ref<Scene> sceneContext;
	};
}