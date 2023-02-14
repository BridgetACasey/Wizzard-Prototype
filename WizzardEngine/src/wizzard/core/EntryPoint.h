//@BridgetACasey

#pragma once

#ifdef WIZZARD_PLATFORM_WINDOWS

extern Wizzard::Application* Wizzard::CreateApplication();

int main(int argc, int** argv)
{
	Wizzard::Log::Init();

	WIZ_PROFILE_BEGIN_SESSION("Startup", "WizzardProfileStartup.json");
	auto app = Wizzard::CreateApplication();
	WIZ_PROFILE_END_SESSION();

	WIZ_PROFILE_BEGIN_SESSION("Runtime", "WizzardProfileRuntime.json");
	app->Run();
	WIZ_PROFILE_END_SESSION();

	WIZ_PROFILE_BEGIN_SESSION("Shutdown", "WizzardProfileShutdown.json");
	delete app;
	WIZ_PROFILE_END_SESSION();

	return 0;
}

#endif