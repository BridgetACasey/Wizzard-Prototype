//@BridgetACasey

#pragma once

#include <string>

namespace Wizzard
{
	class ScriptCommand;

	struct ConditionData
	{
		ConditionData() = default;
	};

	struct CommandData
	{
		CommandData() = default;
	};

	//Basic component exclusively for issuing object commands within the editor in lue of proper scripting
	struct ScriptableComponent
	{
		ScriptableComponent() = default;
		ScriptableComponent(const ScriptableComponent&) = default;

		std::string conditionName = "None";
		std::string commandName = "None";
	};
}