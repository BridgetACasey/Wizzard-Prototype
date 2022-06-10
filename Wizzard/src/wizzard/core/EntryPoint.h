//@BridgetACasey

#pragma once

#ifdef WIZZARD_PLATFORM_WINDOWS

extern Wizzard::Application* Wizzard::createApplication();

int main(int argc, int** argv)
{
	Wizzard::Log::init();

	WIZZARD_WARN("Initialised WIZZARD Engine Log");

	int testNum = 5;
	APP_INFO("Initialised Application Log! Test Number={0}", testNum);

	auto app = Wizzard::createApplication();

	app->run();

	delete app;

	return 0;
}

#endif