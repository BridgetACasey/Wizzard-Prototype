//@BridgetACasey

#include "wzpch.h"

#include "ResourcePathFinder.h"

namespace Wizzard
{
	static std::unordered_map<ResourceType, std::string> resourcePaths;

	void ResourcePath::Init()
	{
		resourcePaths.emplace(ALL, std::string("../Assets/WizzardEngine"));
		resourcePaths.emplace(FONT, std::string("../Assets/WizzardEngine/font"));
		resourcePaths.emplace(MUSIC, std::string("../Assets/WizzardEngine/music"));
		resourcePaths.emplace(SCENE, std::string("../Assets/WizzardEngine/scenes"));
		resourcePaths.emplace(SFX, std::string("../Assets/WizzardEngine/sfx"));
		resourcePaths.emplace(SHADER, std::string("../Assets/WizzardEngine/shaders"));
		resourcePaths.emplace(TEXTURE, std::string("../Assets/WizzardEngine/textures"));
		resourcePaths.emplace(CONFIG, std::string("../Assets/WizzardEngine/config"));
		resourcePaths.emplace(DEBUG, std::string("../Assets/WizzardEngine/debug"));
	}

	void ResourcePath::Shutdown()
	{
		resourcePaths.clear();
	}

	std::string ResourcePath::GetResourcePathForType(ResourceType resourceType)
	{
		return resourcePaths.at(resourceType);
	}

	std::string ResourcePath::GetResourcePath(ResourceType resourceType, const char* resourceName)
	{
		std::string resPath = resourcePaths.at(resourceType);
		resPath.append("/").append(resourceName);
		return resPath;
	}
}