//@BridgetACasey

#pragma once

#include "EditorPanel.h"
#include "wizzard/event/ApplicationEvent.h"

namespace Wizzard
{
	class ApplicationSettingsPanel : public EditorPanel
	{
	public:
		ApplicationSettingsPanel();

		void OnEvent(Event& event) override;
		void OnImGuiRender() override;

	private:
		static bool OnAppShutdown(AppShutdownEvent& appEvent);
	};
}