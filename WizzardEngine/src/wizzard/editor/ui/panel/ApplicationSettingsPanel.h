//@BridgetACasey

#pragma once

#include "EditorPanel.h"

namespace Wizzard
{
	class ApplicationSettingsPanel : public EditorPanel
	{
	public:
		ApplicationSettingsPanel() = default;

		void OnImGuiRender() override;
	};
}