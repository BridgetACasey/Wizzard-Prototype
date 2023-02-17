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

	private:
		entt::registry registry;

		friend class Entity;
	};
}