//@BridgetACasey

#pragma once

#include <string>

namespace Wizzard
{
	struct TagComponent
	{
		std::string tag;
		std::string baseTag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : tag(tag) {}
	};
}