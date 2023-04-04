//@BridgetACasey

#pragma once

#include "wizzard/base/Base.h"
#include "wizzard/scene/Scene.h"
#include "wizzard/editor/ui/screenreading/ScreenReaderLogger.h"

namespace Wizzard
{
	class EditorPanel : public WizRefCounter
	{
	public:
		EditorPanel() = default;

		virtual void SetSceneContext(const WizRef<Scene>& scene);

		virtual void OnUpdate(TimeStep timeStep);
		virtual void OnEvent(Event& event);
		virtual void OnImGuiRender();

		bool IsHovered() const { return isHovered; }
		bool IsFocused() const { return isFocused; }
		//bool ShouldTriggerFocus() const { return shouldTriggerFocus; }

		void SetShouldTriggerFocus(bool focus) { shouldTriggerFocus = focus; }

	protected:
		WizRef<Scene> sceneContext;

		bool isHovered = false;
		bool isFocused = false;
		bool shouldTriggerFocus = false;
	};
}