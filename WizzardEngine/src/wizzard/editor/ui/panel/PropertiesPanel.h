//@BridgetACasey

#pragma once

#include "EditorPanel.h"

namespace Wizzard
{
	class PropertiesPanel : public EditorPanel
	{
	public:
		PropertiesPanel() = default;

		void OnImGuiRender() override;
	};
}