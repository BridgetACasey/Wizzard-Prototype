//@BridgetACasey

#pragma once

#include "wizzard/base/Base.h"
#include "wizzard/scene/Scene.h"

namespace Wizzard
{
	class ObjectCreationPanel
	{
	public:
		ObjectCreationPanel() = default;
		ObjectCreationPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();

	private:
		Ref<Scene> sceneContext;
	};
}