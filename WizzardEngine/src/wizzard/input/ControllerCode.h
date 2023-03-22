//@BridgetACasey

#pragma once

#include <cstdint>
#include <xinput.h>

namespace Wizzard
{
	using ControllerCode = uint16_t;

	namespace Controller
	{
		enum ControllerCode
		{
			None = 999,

			//From glfw3.h				//Descriptions taken from imgui.h
			ButtonDown = 0,				// A (Xbox)         B (Switch)   Cross (PS)
			ButtonRight = 1,			// B (Xbox)         A (Switch)   Circle (PS)
			ButtonLeft = 2,				// X (Xbox)         Y (Switch)   Square (PS)
			ButtonUp = 3,				// Y (Xbox)         X (Switch)   Triangle (PS)

			LeftBumper = 4, 			// L Bumper (Xbox)  L (Switch)   L1 (PS)
			RightBumper = 5,			// R Bumper (Xbox)  R (Switch)   R1 (PS)

			Back = 6,					// View (Xbox)      - (Switch)   Share (PS)
			Start = 7,					// Menu (Xbox)      + (Switch)   Start/Options (PS)

			DPadUp = 11,				// D-Pad Up
			DPadRight = 12,				// D-Pad Right
			DPadDown = 13,				// D-Pad Down
			DPadLeft = 14,				// D-Pad Left

			//TODO: Implement axes
			//LeftTrigger = (1 << 4),			// L Trig. (Xbox)   ZL (Switch)  L2 (PS) [Analog]
			//RightTrigger = (1 << 5),			// R Trig. (Xbox)   ZR (Switch)  R2 (PS) [Analog]
			//
			//LeftAnalogueStick, 		// L Stick (Xbox)   L3 (Switch)  L3 (PS)
			//LeftAnalogueStickUp,
			//LeftAnalogueStickDown, 
			//LeftAnalogueStickLeft,
			//LeftAnalogueStickRight,
			//
			//RightAnalogueStick,		// R Stick (Xbox)   R3 (Switch)  R3 (PS)
			//RightAnalogueStickUp,
			//RightAnalogueStickDown,	
			//RightAnalogueStickLeft,
			//RightAnalogueStickRight,

			/*
			 * Placeholder value for total number of elements in the controller code enum.
			 * Currently does not count 'NONE' value towards total, only considers valid keys.
			 * Must be manually updated.
			 */
			 TotalCodeCount = 24
		};
	}
}