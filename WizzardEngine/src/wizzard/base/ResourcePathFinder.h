//@BridgetACasey

#pragma once

#include <string>

namespace Wizzard
{
	enum ResourceType
	{
		ALL = 0,	//For returning the base resource/asset directory
		FONT,
		MUSIC,
		SCENE,
		SFX,
		SHADER,
		TEXTURE,
		CONFIG,
		DEBUG
	};

	class ResourcePath
	{
	public:
		static void Init();
		static void Shutdown();

		static std::string GetResourcePathForType(ResourceType resourceType);
		//TODO: Add method of returning resource path as const char* directly
		static std::string GetResourcePath(ResourceType resourceType, const char* resourceName);
	};
}