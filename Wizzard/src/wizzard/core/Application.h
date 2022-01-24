//@BridgetACasey

#pragma once

#include "Core.h"

namespace Wizzard
{
	class WIZZARD_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	Application* createApplication();	//To be defined in the client
}