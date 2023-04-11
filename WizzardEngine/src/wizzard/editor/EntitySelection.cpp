//@BridgetACasey

#include "wzpch.h"

#include "EntitySelection.h"

#include <queue>

#include "ui/screenreading/ScreenReaderLogger.h"
#include "wizzard/scene/component/TransformComponent.h"
#include "wizzard/scene/component/BoxCollider2DComponent.h"
#include "wizzard/scene/component/CameraComponent.h"
#include "wizzard/scene/component/CharacterControllerComponent.h"
#include "wizzard/scene/component/RigidBody2DComponent.h"
#include "wizzard/scene/component/SpriteComponent.h"

namespace Wizzard
{
	void EntitySelection::SelectEntity(Entity entity)
	{
		if (!entity)
			return;

		if (IsSelected(entity))
			return;

		selectedEntities.push_back(entity);
	}

	bool EntitySelection::IsSelected(Entity entity)
	{
		return std::find(selectedEntities.begin(), selectedEntities.end(), entity) != selectedEntities.end();
	}

	void EntitySelection::DeselectEntity(Entity entity)
	{
		auto itr = std::find(selectedEntities.begin(), selectedEntities.end(), entity);

		if (itr == selectedEntities.end())
			return;

		selectedEntities.erase(itr);
	}

	void EntitySelection::DeselectAll()
	{
		if(!selectedEntities.empty())
		selectedEntities.clear();
	}

	void EntitySelection::DescribeSelectionsIsolated()
	{
		if(!selectedEntities.empty())
		{
			ScreenReaderLogger::Stop();

			ScreenReaderLogger::QueueOutput("Total " + std::to_string(selectedEntities.size()) + " selected entities.", true, true);

			for(auto entity : selectedEntities)
			{
				std::string name = entity.GetName();
				auto transform = entity.GetComponent<TransformComponent>();
				//auto boxCollider = entity.GetComponent<BoxCollider2DComponent>();
				//auto rigidBody = entity.GetComponent<RigidBody2DComponent>();
				//auto camera = entity.GetComponent<CameraComponent>();
				//auto character = entity.GetComponent<CharacterControllerComponent>();

				ScreenReaderLogger::QueueOutput(name + " at X " + std::to_string((int)transform.Translation.x) + ", Y " + std::to_string((int)transform.Translation.y), false, true);

				if (entity.HasComponent<SpriteComponent>())
				{
					auto sprite = entity.GetComponent<SpriteComponent>();
					ScreenReaderLogger::QueueOutput(name + " has sprite component. Describe colours here.", false, true);
				}
			}
		}
		else
			ScreenReaderLogger::QueueOutput("No selected entities.", false, true);
	}

	void EntitySelection::DescribeSelectionsRelational()
	{
		if (!selectedEntities.empty())
		{
			ScreenReaderLogger::Stop();

			std::string message = "Total " + std::to_string(selectedEntities.size()) + " selected entities. ";
			//ScreenReaderLogger::QueueOutput("Total " + std::to_string(selectedEntities.size()) + " selected entities.", false, true);

			//Create unique pairs of objects. Entities should not be paired with themselves and being paired in reverse order should not count.
			std::vector<std::pair<Entity, Entity>> entityPairs;
			std::vector<entt::entity> handledEntityIndices;

			for (auto firstEntity : selectedEntities)
			{
				for(auto secondEntity : selectedEntities)
				{
					if (firstEntity == secondEntity)
						continue;

					if (std::find(handledEntityIndices.begin(), handledEntityIndices.end(), secondEntity.GetEntityHandle()) != handledEntityIndices.end())
						continue;

					entityPairs.emplace_back(std::pair(firstEntity, secondEntity));
				}

				handledEntityIndices.push_back(firstEntity.GetEntityHandle());
			}

			for(auto entityPair : entityPairs)
			{
				std::string nameFirst = entityPair.first.GetName();
				auto transformFirst = entityPair.first.GetComponent<TransformComponent>();

				std::string nameSecond = entityPair.second.GetName();
				auto transformSecond = entityPair.second.GetComponent<TransformComponent>();

				//auto boxCollider = entity.GetComponent<BoxCollider2DComponent>();
				//auto rigidBody = entity.GetComponent<RigidBody2DComponent>();
				//auto camera = entity.GetComponent<CameraComponent>();
				//auto character = entity.GetComponent<CharacterControllerComponent>();

				//ScreenReaderLogger::QueueOutput(nameFirst + " at X " + std::to_string((int)transformFirst.Translation.x) + ", Y " + std::to_string((int)transformFirst.Translation.y)
				//	+ " paired with " + nameSecond + " at X " + std::to_string((int)transformSecond.Translation.x) + ", Y " + std::to_string((int)transformSecond.Translation.y), false, true);

				//message.append(nameFirst + " at X " + std::to_string((int)transformFirst.Translation.x) + ", Y " + std::to_string((int)transformFirst.Translation.y)
				//	+ " paired with " + nameSecond + " at X " + std::to_string((int)transformSecond.Translation.x) + ", Y " + std::to_string((int)transformSecond.Translation.y));

				float spaceDiff = 0.01f;

				//Describe positions in relation to each other
				{
					float xDiff = abs(transformFirst.Translation.x - transformSecond.Translation.x);
					float yDiff = abs(transformFirst.Translation.y - transformSecond.Translation.y);

					if ((transformFirst.Translation.x - transformSecond.Translation.x) > spaceDiff)
						//ScreenReaderLogger::QueueOutput(nameFirst + " is to the right of " + nameSecond + " by " + std::to_string(floor(xDiff * 100.0f) / 100.0f) + " units. ", false, true);
						message.append(nameFirst + " is to the right of " + nameSecond + " by " + std::to_string(floor(xDiff * 100.0f) / 100.0f) + " units. ");
					else if ((transformFirst.Translation.x - transformSecond.Translation.x) < -spaceDiff)
						//ScreenReaderLogger::QueueOutput(nameFirst + " is to the left of " + nameSecond + " by " + std::to_string(floor(xDiff * 100.0f) / 100.0f) + " units. ", false, true);
						message.append(nameFirst + " is to the left of " + nameSecond + " by " + std::to_string(floor(xDiff * 100.0f) / 100.0f) + " units. ");
					
					if ((transformFirst.Translation.y - transformSecond.Translation.y) > spaceDiff)
						//ScreenReaderLogger::QueueOutput(nameFirst + " is above " + nameSecond + " by " + std::to_string(floor(yDiff * 100.0f) / 100.0f) + " units. ", false, true);
						message.append(nameFirst + " is above " + nameSecond + " by " + std::to_string(floor(yDiff * 100.0f) / 100.0f) + " units. ");
					else if ((transformFirst.Translation.y - transformSecond.Translation.y) < -spaceDiff)
						//ScreenReaderLogger::QueueOutput(nameFirst + " is below " + nameSecond + " by " + std::to_string(floor(yDiff * 100.0f) / 100.0f) + " units. ", false, true);
						message.append(nameFirst + " is below " + nameSecond + " by " + std::to_string(floor(yDiff * 100.0f) / 100.0f) + " units. ");
				}

				//Describe collision details. Are they overlapping or not? In what way? At edges, corners, partially intersected, or exactly on top of each other?
				if (entityPair.first.HasComponent<BoxCollider2DComponent>() && entityPair.second.HasComponent<BoxCollider2DComponent>())
				{
					auto firstCol = entityPair.first.GetComponent<BoxCollider2DComponent>();
					auto secondCol = entityPair.first.GetComponent<BoxCollider2DComponent>();

					float first[8] =
					{
						transformFirst.Translation.x - (firstCol.Size.x / 2.0f),	//Left Edge Centre
						transformFirst.Translation.x + (firstCol.Size.x / 2.0f),	//Right Edge Centre
						transformFirst.Translation.y - (firstCol.Size.y / 2.0f),	//Bottom Edge Centre
						transformFirst.Translation.y + (firstCol.Size.y / 2.0f),	//Top Edge Centre
						transformFirst.Translation.x - (firstCol.Size.x / 2.0f) - (firstCol.Size.y / 2.0f),	//Bottom Left Corner
						transformFirst.Translation.x + (firstCol.Size.x / 2.0f) + (firstCol.Size.y / 2.0f),	//Top Right Corner
						transformFirst.Translation.y - (firstCol.Size.y / 2.0f) + (firstCol.Size.x / 2.0f),	//Bottom Right Corner
						transformFirst.Translation.y + (firstCol.Size.y / 2.0f) - (firstCol.Size.x / 2.0f),	//Top Left Corner
					};

					float second[8] =
					{
						transformSecond.Translation.x - (secondCol.Size.x / 2.0f),	//Left Edge Centre
						transformSecond.Translation.x + (secondCol.Size.x / 2.0f),	//Right Edge Centre
						transformSecond.Translation.y - (secondCol.Size.y / 2.0f),	//Bottom Edge Centre
						transformSecond.Translation.y + (secondCol.Size.y / 2.0f),	//Top Edge Centre
						transformSecond.Translation.x - (secondCol.Size.x / 2.0f) - (secondCol.Size.y / 2.0f),	//Bottom Left Corner
						transformSecond.Translation.x + (secondCol.Size.x / 2.0f) + (secondCol.Size.y / 2.0f),	//Top Right Corner
						transformSecond.Translation.y - (secondCol.Size.y / 2.0f) + (secondCol.Size.x / 2.0f),	//Bottom Right Corner
						transformSecond.Translation.y + (secondCol.Size.y / 2.0f) - (secondCol.Size.x / 2.0f),	//Top Left Corner
					};

					//if ((abs(second[1]) - abs(first[0])) > spaceDiff && (abs(first[0]) - abs(second[0])) < spaceDiff)
					if(first[1] - second[0] < spaceDiff && first[0] > second[0])
					{
						//ScreenReaderLogger::QueueOutput("The left edge of " + nameFirst + " is overlapping the right edge of " + nameSecond, false, true);
						message.append("The left edge of " + nameFirst + " is overlapping the right edge of " + nameSecond + ". ");
					}
					//else if ((abs(first[1]) - abs(second[0])) > spaceDiff && (abs(second[0]) - abs(first[0])) < spaceDiff)
					else if (first[0] - second[1] < spaceDiff && second[0] > first[0])
					{
						//ScreenReaderLogger::QueueOutput("The left edge of " + nameSecond + " is overlapping the right edge of " + nameFirst, false, true);
						message.append("The left edge of " + nameSecond + " is overlapping the right edge of " + nameFirst + ". ");
					}
				
					//if ((abs(second[0]) - abs(first[1])) > spaceDiff && (abs(first[1]) - abs(second[1])) < spaceDiff)
					if (second[1] - first[0] < spaceDiff && second[1] > first[1])
					{
						//ScreenReaderLogger::QueueOutput("The right edge of " + nameSecond + " is overlapping the left edge of " + nameFirst, false, true);
						message.append("The right edge of " + nameSecond + " is overlapping the left edge of " + nameFirst + ". ");
					}
					//else if ((abs(first[0]) - abs(second[1])) > spaceDiff && (abs(second[1]) - abs(first[1])) < spaceDiff)
					else if (second[0] - first[1] < spaceDiff && first[1] > second[1])
					{
						//ScreenReaderLogger::QueueOutput("The right edge of " + nameFirst + " is overlapping the left edge of " + nameSecond, false, true);
						message.append("The right edge of " + nameFirst + " is overlapping the left edge of " + nameSecond + ". ");
					}

					if ((abs(second[3]) - abs(first[2])) > spaceDiff && (abs(first[2]) - abs(second[2])) < spaceDiff)
						//ScreenReaderLogger::QueueOutput("The top edge of " + nameFirst + " is overlapping the bottom edge of " + nameSecond, false, true);
						message.append("The top edge of " + nameFirst + " is overlapping the bottom edge of " + nameSecond);

					else if ((abs(first[3]) - abs(second[2])) > spaceDiff && (abs(second[2]) - abs(first[2])) < spaceDiff)
						//ScreenReaderLogger::QueueOutput("The top edge of " + nameSecond + " is overlapping the bottom edge of " + nameFirst, false, true);
						message.append("The top edge of " + nameSecond + " is overlapping the bottom edge of " + nameFirst);

					if ((abs(second[2]) - abs(first[3])) > spaceDiff && (abs(first[3]) - abs(second[3])) < spaceDiff)
						//ScreenReaderLogger::QueueOutput("The top edge of " + nameSecond + " is overlapping the bottom edge of " + nameFirst, false, true);
						message.append("The top edge of " + nameSecond + " is overlapping the bottom edge of " + nameFirst);

					else if ((abs(first[2]) - abs(second[3])) > spaceDiff && (abs(second[3]) - abs(first[3])) < spaceDiff)
						//ScreenReaderLogger::QueueOutput("The top edge of " + nameFirst + " is overlapping the bottom edge of " + nameSecond, false, true);
						message.append("The top edge of " + nameFirst + " is overlapping the bottom edge of " + nameSecond);
				}
			}

			ScreenReaderLogger::QueueOutput(message, true, true);
		}
		else
			ScreenReaderLogger::QueueOutput("No selected entities.", true, true);
	}
}