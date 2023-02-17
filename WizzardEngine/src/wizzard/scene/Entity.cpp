//@BridgetACasey

#include "wzpch.h"
#include "Entity.h"

namespace Wizzard
{
	Entity::Entity(entt::entity handle, Scene* scene) : entityHandle(handle), scene(scene)
	{
	}
}