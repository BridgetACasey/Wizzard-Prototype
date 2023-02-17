//@BridgetACasey

#include "wzpch.h"
#include "Scene.h"

#include "Entity.h"
#include "TagComponent.h"
#include "TransformComponent.h"

namespace Wizzard
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();

		auto& tagComponent = entity.AddComponent<TagComponent>();
		tagComponent.tag = name.empty() ? "Entity" : name;

		return entity;
	}

	void Scene::OnUpdate(TimeStep timeStep)
	{
	}
}