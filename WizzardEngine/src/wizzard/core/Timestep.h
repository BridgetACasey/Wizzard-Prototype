//@BridgetACasey

#pragma once

namespace Wizzard
{
	class Timestep
	{
	public:
		Timestep(float t = 0.0f) : time(t)
		{
			WIZ_PROFILE_FUNCTION();
		}

		operator float() const { return time; }

		float GetSeconds() const { return time; }

		float GetMilliseconds() const { return time * 1000.0f; }

	private:
		float time;
	};
}