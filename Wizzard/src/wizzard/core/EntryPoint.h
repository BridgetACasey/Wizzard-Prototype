//@BridgetACasey

#pragma once

#ifdef WIZZARD_PLATFORM_WINDOWS

extern Wizzard::Application* Wizzard::createApplication();

int main(int argc, int** argv)
{
	Wizzard::Log::init();

	WIZZARD_INFO("Initialised WIZZARD Engine Log");

	auto app = Wizzard::createApplication();

	app->run();

	delete app;

	return 0;
}

#endif