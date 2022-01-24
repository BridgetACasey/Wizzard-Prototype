//@BridgetACasey

#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Wizzard
{
	class WIZZARD_API Log
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
#define LOG_WIZZARD_TRACE(...)	::Engine::Log::getWizzardLogger()->trace(__VA_ARGS__)
#define LOG_WIZZARD_INFO(...)	::Engine::Log::getWizzardLogger()->info(__VA_ARGS__)
#define LOG_WIZZARD_WARN(...)	::Engine::Log::getWizzardLogger()->warn(__VA_ARGS__)
#define LOG_WIZZARD_ERROR(...)	::Engine::Log::getWizzardLogger()->error(__VA_ARGS__)
#define LOG_WIZZARD_FATAL(...)	::Engine::Log::getWizzardLogger()->fatal(__VA_ARGS__)

//Application logger macros
#define LOG_APP_TRACE(...)	::Engine::Log::getAppLogger()->trace(__VA_ARGS__)
#define LOG_APP_INFO(...)	::Engine::Log::getAppLogger()->info(__VA_ARGS__)
#define LOG_APP_WARN(...)	::Engine::Log::getAppLogger()->warn(__VA_ARGS__)
#define LOG_APP_ERROR(...)	::Engine::Log::getAppLogger()->error(__VA_ARGS__)
#define LOG_APP_FATAL(...)	::Engine::Log::getAppLogger()->fatal(__VA_ARGS__)