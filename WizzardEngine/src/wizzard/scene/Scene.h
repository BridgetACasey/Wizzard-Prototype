//@BridgetACasey

#pragma once

#include "entt/entt.hpp"

#include "wizzard/base/Timestep.h"
#include "wizzard/rendering/EditorCamera.h"

class b2World;

namespace Wizzard
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnStart();
		void OnStop();
		void OnUpdateRuntime(TimeStep timeStep);
		void OnUpdateEditor(TimeStep timeStep, EditorCamera& camera);

		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		Entity GetPrimaryCameraEntity();

		void OnViewportResize(uint32_t width, uint32_t height);

	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);

		entt::registry registry;

		uint32_t viewportWidth = 0, viewportHeight = 0;

		b2World* m_PhysicsWorld = nullptr;

		friend class Entity;
		friend class SceneSerialiser;
		friend class SceneHierarchyPanel;
	};
}
