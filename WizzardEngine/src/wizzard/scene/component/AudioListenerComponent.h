//@BridgetACasey

#pragma once

namespace Wizzard
{
	struct AudioListenerComponent
	{
		AudioListenerComponent() = default;
		AudioListenerComponent(const AudioListenerComponent&) = default;

		bool isActive = false;
	};
}