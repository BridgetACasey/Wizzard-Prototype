//@BridgetACasey

#pragma once

#include "EditorPanel.h"

namespace Wizzard
{
	class ObjectCreationPanel : public EditorPanel
	{
	public:
		ObjectCreationPanel() = default;

		void OnImGuiRender() override;
	};
}