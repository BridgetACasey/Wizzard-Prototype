//@BridgetACasey

#pragma once

#include "wizzard/core/Input.h"

namespace Wizzard
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(KeyCodeGL keycode) override;
		virtual bool IsMouseButtonPressedImpl(MouseCodeGL button) override;

		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}