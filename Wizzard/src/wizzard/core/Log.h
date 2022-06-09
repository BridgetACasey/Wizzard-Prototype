//@BridgetACasey

#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Wizzard
{
	class Log
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger>& getWizzardLogger() { return s_wizzardLogger; }
		inline static std::shared_ptr<spdlog::logger>& getAppLogger() { return s_appLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_wizzardLogger;
		static std::shared_ptr<spdlog::logger> s_appLogger;
	};
}

//Wizzard Engine logger macros
#define WIZZARD_TRACE(...)	::Wizzard::Log::getWizzardLogger()->trace(__VA_ARGS__)
#define WIZZARD_INFO(...)	::Wizzard::Log::getWizzardLogger()->info(__VA_ARGS__)
#define WIZZARD_WARN(...)	::Wizzard::Log::getWizzardLogger()->warn(__VA_ARGS__)
#define WIZZARD_ERROR(...)	::Wizzard::Log::getWizzardLogger()->error(__VA_ARGS__)
#define WIZZARD_FATAL(...)	::Wizzard::Log::getWizzardLogger()->fatal(__VA_ARGS__)

//Application logger macros
#define APP_TRACE(...)	::Wizzard::Log::getAppLogger()->trace(__VA_ARGS__)
#define APP_INFO(...)	::Wizzard::Log::getAppLogger()->info(__VA_ARGS__)
#define APP_WARN(...)	::Wizzard::Log::getAppLogger()->warn(__VA_ARGS__)
#define APP_ERROR(...)	::Wizzard::Log::getAppLogger()->error(__VA_ARGS__)
#define APP_FATAL(...)	::Wizzard::Log::getAppLogger()->fatal(__VA_ARGS__)