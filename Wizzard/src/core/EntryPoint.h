//@BridgetACasey

#pragma once

#ifdef WIZZARD_PLATFORM_WINDOWS

extern Wizzard::Application* Wizzard::createApplication();

int main(int argc, int** argv)
{
	Wizzard::Log::init();
	LOG_ENGINE_WARN("Initialised Engine Log!");
	int appleCount = 5;
	LOG_GAME_INFO("Initialised Application Log! Apples={0}", appleCount);

	auto app = Wizzard::createApplication();

	app->run();

	delete app;

	return 0;
}

#endif