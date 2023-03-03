//@BridgetACasey

#pragma once

#ifdef WIZZARD_PLATFORM_WINDOWS

#include "ResourcePathFinder.h"

extern Wizzard::Application* Wizzard::CreateApplication();

int main(int argc, int** argv)
{
	Wizzard::Log::Init();
	Wizzard::ResourcePath::Init();

	WIZ_PROFILE_BEGIN_SESSION("Startup", Wizzard::ResourcePath::GetResourcePath(Wizzard::DEBUG, "WizzardProfileStartup.json"));
	auto app = Wizzard::CreateApplication();
	WIZ_PROFILE_END_SESSION();

	WIZ_PROFILE_BEGIN_SESSION("Runtime", Wizzard::ResourcePath::GetResourcePath(Wizzard::DEBUG, "WizzardProfileRuntime.json"));
	app->Run();
	WIZ_PROFILE_END_SESSION();

	WIZ_PROFILE_BEGIN_SESSION("Shutdown", Wizzard::ResourcePath::GetResourcePath(Wizzard::DEBUG, "WizzardProfileShutdown.json"));
	delete app;
	WIZ_PROFILE_END_SESSION();

	Wizzard::ResourcePath::Shutdown();

	return 0;
}

#endif