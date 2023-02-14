//@BridgetACasey

#include "wzpch.h"
#include "Layer.h"

namespace Wizzard
{
	Layer::Layer(const std::string& dbName) : debugName(dbName)
	{
		WIZ_PROFILE_FUNCTION();
	}

	Layer::~Layer()
	{
		WIZ_PROFILE_FUNCTION();
	}
}