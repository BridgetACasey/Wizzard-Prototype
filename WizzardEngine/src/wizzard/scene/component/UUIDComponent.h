//@BridgetACasey

#pragma once

#include "wizzard/common/UUID.h"

namespace Wizzard
{
	struct UUIDComponent
	{
		UUID uuid;

		UUIDComponent() = default;
		UUIDComponent(const UUIDComponent& id) = default;
	};
}