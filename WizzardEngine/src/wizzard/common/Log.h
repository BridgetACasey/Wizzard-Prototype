//@BridgetACasey

#pragma once

#include "wizzard/base/Base.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#define SPDLOG_WCHAR_TO_UTF8_SUPPORT

namespace Wizzard
{
	class Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetWizzardLogger() { return wizzardLogger; }
		static std::shared_ptr<spdlog::logger>& GetLuggageLogger() { return luggageLogger; }
		static std::shared_ptr<spdlog::logger>& GetAppLogger() { return appLogger; }

	private:
		static std::shared_ptr<spdlog::logger> wizzardLogger;
		static std::shared_ptr<spdlog::logger> luggageLogger;
		static std::shared_ptr<spdlog::logger> appLogger;
	};
}

//Wizzard Engine logger macros
#define WIZ_TRACE(...)	::Wizzard::Log::GetWizzardLogger()->trace(__VA_ARGS__)
#define WIZ_INFO(...)	::Wizzard::Log::GetWizzardLogger()->info(__VA_ARGS__)
#define WIZ_WARN(...)	::Wizzard::Log::GetWizzardLogger()->warn(__VA_ARGS__)
#define WIZ_ERROR(...)	::Wizzard::Log::GetWizzardLogger()->error(__VA_ARGS__)
#define WIZ_FATAL(...)	::Wizzard::Log::GetWizzardLogger()->fatal(__VA_ARGS__)

//Wizzard Engine logger macros
#define LUG_TRACE(...)	::Wizzard::Log::GetLuggageLogger()->trace(__VA_ARGS__)
#define LUG_INFO(...)	::Wizzard::Log::GetLuggageLogger()->info(__VA_ARGS__)
#define LUG_WARN(...)	::Wizzard::Log::GetLuggageLogger()->warn(__VA_ARGS__)
#define LUG_ERROR(...)	::Wizzard::Log::GetLuggageLogger()->error(__VA_ARGS__)
#define LUG_FATAL(...)	::Wizzard::Log::GetLuggageLogger()->fatal(__VA_ARGS__)

//Application logger macros
#define APP_TRACE(...)	::Wizzard::Log::GetAppLogger()->trace(__VA_ARGS__)
#define APP_INFO(...)	::Wizzard::Log::GetAppLogger()->info(__VA_ARGS__)
#define APP_WARN(...)	::Wizzard::Log::GetAppLogger()->warn(__VA_ARGS__)
#define APP_ERROR(...)	::Wizzard::Log::GetAppLogger()->error(__VA_ARGS__)
#define APP_FATAL(...)	::Wizzard::Log::GetAppLogger()->fatal(__VA_ARGS__)