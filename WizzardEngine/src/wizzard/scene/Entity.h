//@BridgetACasey

#pragma once

#include "Scene.h"

#include "entt/entt.hpp"

namespace Wizzard
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			WIZ_ASSERT(!HasComponent<T>(), "Entity already has component!");
			return scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			WIZ_ASSERT(HasComponent<T>(), "Entity does not have component!");
			return scene->registry.get<T>(entityHandle);
		}

		template<typename T>
		bool HasComponent()
		{
			return scene->registry.has<T>(entityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			WIZ_ASSERT(HasComponent<T>(), "Entity does not have component!");
			scene->registry.remove<T>(entityHandle);
		}

		operator bool() const { return entityHandle != entt::null; }

	private:
		entt::entity entityHandle{ 0 };

		Scene* scene = nullptr;
	};

}