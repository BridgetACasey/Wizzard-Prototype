//@BridgetACasey

#pragma once

#include "wizzard/core/Core.h"

namespace Wizzard
{
	class Input
	{
	public:
		static bool IsKeyPressed(int keycode) { return instance->IsKeyPressedImpl(keycode); }

		static bool IsMouseButtonPressed(int button) { return instance->IsMouseButtonPressedImpl(button); }
		static std::pair<float, float> GetMousePosition() { return instance->GetMousePositionImpl(); }
		static float GetMouseX() { return instance->GetMouseXImpl(); }
		static float GetMouseY() { return instance->GetMouseYImpl(); }
	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;

		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
	private:
		static Input* instance;
	};
}