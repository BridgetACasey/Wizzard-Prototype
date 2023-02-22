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

		void OnUpdate(TimeStep timeStep);

		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		entt::registry registry;

		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

		friend class Entity;
	};
}