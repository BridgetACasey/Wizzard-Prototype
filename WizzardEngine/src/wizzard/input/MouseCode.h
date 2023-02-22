//@BridgetACasey

#pragma once

#include <cstdint>

namespace Wizzard
{
	using MouseCode = uint16_t;

	namespace Mouse
	{
		enum : MouseCode
		{
			None = 999,

			// From glfw3.h
			Button0 = 0,
			Button1 = 1,
			Button2 = 2,
			Button3 = 3,
			Button4 = 4,
			Button5 = 5,
			Button6 = 6,
			Button7 = 7,

			LeftButton   = Button0,
			RightButton	 = Button1,
			MiddleButton = Button2,
			LastButton = Button7,

			/*
			 * Placeholder value for total number of elements in the mouse code enum.
			 * Currently does not count 'NONE' value towards total, only considers valid keys.
			 * Must be manually updated.
			 */
			TotalCodeCount = 11
		};
	}
}