//@BridgetACasey

#include "wzpch.h"

#include "Log.h"

namespace Wizzard
{
	std::shared_ptr<spdlog::logger> Log::wizzardLogger;
	std::shared_ptr<spdlog::logger> Log::appLogger;

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T][%n][%l]: %v%$");
		wizzardLogger = spdlog::stdout_color_mt("ENGINE");
		wizzardLogger->set_level(spdlog::level::trace);

		appLogger = spdlog::stdout_color_mt("APP");
		appLogger->set_level(spdlog::level::trace);
	}
}