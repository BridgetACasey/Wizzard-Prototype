//@BridgetACasey

#pragma once

#include "EditorPanel.h"
#include "wizzard/event/AudioEvent.h"

namespace Wizzard
{
	class ApplicationSettingsPanel : public EditorPanel
	{
	public:
		ApplicationSettingsPanel() = default;

		void OnEvent(Event& event) override;
		void OnImGuiRender() override;

	private:
		static bool OnScreenReaderMessageEnded(ScreenReaderMessageEndedEvent& srEvent);
	};
}
