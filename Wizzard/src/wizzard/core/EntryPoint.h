//@BridgetACasey

#pragma once

#ifdef WIZZARD_PLATFORM_WINDOWS

extern Wizzard::Application* Wizzard::createApplication();

int main(int argc, int** argv)
{
	Wizzard::Log::init();

	LOG_WIZZARD_WARN("Initialised Engine Log!");

	int testNum = 5;

	LOG_APP_INFO("Initialised Application Log! Test Number={0}", testNum);

	auto app = Wizzard::createApplication();

	app->run();

	delete app;

	return 0;
}

#endif