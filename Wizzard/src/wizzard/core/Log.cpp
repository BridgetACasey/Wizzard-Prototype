//@BridgetACasey

#include "wzpch.h"

#include "Log.h"

namespace Wizzard
{
	std::shared_ptr<spdlog::logger> Log::s_wizzardLogger;
	std::shared_ptr<spdlog::logger> Log::s_appLogger;

	void Log::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		s_wizzardLogger = spdlog::stdout_color_mt("Wizzard");
		s_wizzardLogger->set_level(spdlog::level::trace);

		s_appLogger = spdlog::stdout_color_mt("Application");
		s_appLogger->set_level(spdlog::level::trace);
	}
}