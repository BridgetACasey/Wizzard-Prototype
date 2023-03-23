//@BridgetACasey

#include "wzpch.h"

#include "Reference.h"

namespace Wizzard
{
	static std::unordered_set<void*> activeReferences;

	template <typename T>
	void WizRef<T>::AddActiveReference(void* instance)
	{
		if(instance)
		activeReferences.insert(instance);
	}

	template<typename T>
	void WizRef<T>::EraseActiveReference(void* instance)
	{
		if (instance)
		{
			if (IsActive(instance))
				activeReferences.erase(instance);
		}
	}

	template<typename T>
	bool WizRef<T>::IsActive(void* instance)
	{
		return activeReferences.find(instance) != activeReferences.end();
	}
}