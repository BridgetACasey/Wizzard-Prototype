//@BridgetACasey

#include "wzpch.h"

#include "ResourcePathFinder.h"

namespace Wizzard
{
	static std::unordered_map<ResourceType, std::string> resourcePaths;

	void ResourcePath::Init()
	{
		resourcePaths.emplace(ALL, std::string("res"));
		resourcePaths.emplace(FONT, std::string("res/font"));
		resourcePaths.emplace(MUSIC, std::string("res/music"));
		resourcePaths.emplace(SCENE, std::string("res/scenes"));
		resourcePaths.emplace(SFX, std::string("res/sfx"));
		resourcePaths.emplace(SHADER, std::string("res/shaders"));
		resourcePaths.emplace(TEXTURE, std::string("res/textures"));
		resourcePaths.emplace(CONFIG, std::string("res/config"));
		resourcePaths.emplace(DEBUG, std::string("res/debug"));
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