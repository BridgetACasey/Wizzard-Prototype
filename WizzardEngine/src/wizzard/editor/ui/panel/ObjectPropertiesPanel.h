//@BridgetACasey

#pragma once

#include "EditorPanel.h"

namespace Wizzard
{
	class ObjectPropertiesPanel : public EditorPanel
	{
	public:
		ObjectPropertiesPanel() = default;

		void OnImGuiRender() override;
	};
}