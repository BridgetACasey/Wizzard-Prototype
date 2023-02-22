//@BridgetACasey

#pragma once

#include "entt/entt.hpp"

#include "wizzard/base/Timestep.h"

namespace Wizzard
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnUpdate(TimeStep timeStep);

		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		entt::registry registry;

		uint32_t viewportWidth = 0, viewportHeight = 0;

		friend class Entity;
		friend class SceneSerialiser;
	};
}