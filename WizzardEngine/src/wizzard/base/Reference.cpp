//@BridgetACasey

#include "wzpch.h"

#include "Reference.h"

namespace Wizzard
{
	static std::unordered_set<void*> activeReferences;

	void WizRefManager::AddActiveReference(void* instance)
	{
		if (instance)
			activeReferences.insert(instance);
	}

	void WizRefManager::EraseActiveReference(void* instance)
	{
		if (instance)
		{
			if (IsActive(instance))
				activeReferences.erase(instance);
		}
	}

	bool WizRefManager::IsActive(void* instance)
	{
		return activeReferences.find(instance) != activeReferences.end();
	}
}