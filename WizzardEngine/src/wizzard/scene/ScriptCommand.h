//@BridgetACasey

#pragma once

#include <string>
#include <unordered_map>

#include "glm/vec3.hpp"

#include <scene/component/ScriptableComponent.h>

namespace Wizzard
{
	class Entity;

	struct ResetPositionCommandData : CommandData
	{
		
	};

	class ScriptCommand
	{
	public:
		static bool ConditionOnCollision(Entity& source, Entity& other);	//On collision begin with another entity

		static void CommandResetPlayer(Entity& entity);
		//static void MoveBetweenPoints(Entity& entity, glm::vec3 start, glm::vec3 end);
		//Change colour?
		//Emit single play sound effect? Probably requires audio component
		//Fall/change rigidbody type? Obvs requires rigid body component
		//Speak to screen reader?

		static void InsertFunction()
		{
			
		}

	private:
		//static inline std::unordered_map<std::string, std::function<ScriptCommand>> functionsByName;
	};
}
