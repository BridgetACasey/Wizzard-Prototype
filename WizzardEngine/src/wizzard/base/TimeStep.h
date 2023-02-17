//@BridgetACasey

#pragma once

namespace Wizzard
{
	class TimeStep
	{
	public:
		TimeStep(float time = 0.0f) : time(time)
		{
		}

		operator float() const { return time; }

		float GetSeconds() const { return time; }

		float GetMilliseconds() const { return time * 1000.0f; }

		float GetMicroseconds() const { return time * 1000000.0f; }

		float GetNanoseconds() const { return time * 1000000000.0f; }

	private:
		float time;	//Time in seconds
	};
}