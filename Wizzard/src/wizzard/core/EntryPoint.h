//@BridgetACasey

#pragma once

#ifdef WIZZARD_PLATFORM_WINDOWS

extern Wizzard::Application* Wizzard::CreateApplication();

int main(int argc, int** argv)
{
	Wizzard::Log::Init();

	WIZ_INFO("Initialised WIZZARD Engine Log");

	auto app = Wizzard::CreateApplication();

	app->Run();

	delete app;

	return 0;
}

#endif