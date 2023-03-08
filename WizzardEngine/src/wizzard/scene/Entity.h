//@BridgetACasey

#pragma once

#include "Scene.h"

#include "entt/entt.hpp"

#include "wizzard/common/UUID.h"
#include "component/UUIDComponent.h"
#include "component/TagComponent.h"

namespace Wizzard
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		UUID GetUUID() { return GetComponent<UUIDComponent>().uuid; }
		const std::string& GetName() { return GetComponent<TagComponent>().tag; }

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			WIZ_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = scene->registry.emplace<T>(entityHandle, std::forward<Args>(args)...);
			scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = scene->registry.emplace_or_replace<T>(entityHandle, std::forward<Args>(args)...);
			scene->OnComponentAdded<T>(*this, component);
			return component;
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
			return scene->registry.any_of<T>(entityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			WIZ_ASSERT(HasComponent<T>(), "Entity does not have component!");
			scene->registry.remove<T>(entityHandle);
		}

		operator bool() const { return entityHandle != entt::null; }
		operator entt::entity() const { return entityHandle; }
		operator uint32_t() const { return (uint32_t)entityHandle; }

		bool operator==(const Entity& other) const
		{
			return entityHandle == other.entityHandle && scene == other.scene;
		}

		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}

	private:
		entt::entity entityHandle{ 0 };

		Scene* scene = nullptr;
	};

}