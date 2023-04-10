//@BridgetACasey

#pragma once

#include "Scene.h"

namespace Wizzard
{
	class SceneManager
	{
	public:
		static void SetActiveScene(const WizRef<Scene>& scene) { activeScene = scene; }
		static WizRef<Scene>& GetActiveScene() { return activeScene; }

	private:
		static inline WizRef<Scene> activeScene;
	};
}