//@BridgetACasey

#pragma once

#include "wizzard/base/Base.h"
#include "wizzard/scene/Scene.h"
#include "wizzard/scene/Entity.h"

namespace Wizzard
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void SetContext(const Ref<Scene>& scene);

		void OnImGuiRender();
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}