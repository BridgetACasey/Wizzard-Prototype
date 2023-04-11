//@BridgetACasey

#include <wzpch.h>
#include "ScriptCommand.h"

#include "Entity.h"
#include "box2d/b2_body.h"
#include "box2d/b2_contact.h"
#include "component/CharacterControllerComponent.h"
#include "component/RigidBody2DComponent.h"
#include "component/TransformComponent.h"
#include "wizzard/editor/ui/screenreading/ScreenReaderLogger.h"

namespace Wizzard
{
	bool ScriptCommand::ConditionOnCollision(Entity& source, Entity& other)
	{
		if (source.HasComponent<RigidBody2DComponent>() && other.HasComponent<RigidBody2DComponent>())
		{
			auto& sourceRb2d = source.GetComponent<RigidBody2DComponent>();
			auto& otherRb2d = other.GetComponent<RigidBody2DComponent>();

			b2Body* sourceBody = (b2Body*)sourceRb2d.RuntimeBody;
			b2Body* otherBody = (b2Body*)otherRb2d.RuntimeBody;

			if(sourceBody->GetContactList() != nullptr)
			{
				if (sourceBody->GetFixtureList()->GetAABB(0).Contains(otherBody->GetFixtureList()->GetAABB(0)))
					return true;
			}
		}

		return false;
	}

	void ScriptCommand::CommandResetPlayer(Entity& entity)
	{
		if (entity.HasComponent<RigidBody2DComponent>())
		{
			auto& transform = entity.GetComponent<TransformComponent>();
			auto& rb2d = entity.GetComponent<RigidBody2DComponent>();

			b2Body* body = (b2Body*)rb2d.RuntimeBody;
			const auto& position = body->GetPosition();
			//transform.Translation.x = position.x;
			//transform.Translation.y = position.y;
			//transform.Rotation.z = body->GetAngle();

			if (entity.HasComponent<CharacterControllerComponent>())
			{
				auto cc = entity.GetComponent<CharacterControllerComponent>();

				ScreenReaderLogger::QueueOutput("Resetting player position");
				body->SetTransform(b2Vec2(cc.defaultPosition.x, cc.defaultPosition.y), 0.0f);
				transform.Translation.x = position.x;
				transform.Translation.y = position.y;
				transform.Rotation.z = body->GetAngle();
			}
		}
	}

	//void ScriptCommand::MoveBetweenPoints(Entity& entity, glm::vec3 start, glm::vec3 end)
	//{
	//}
}