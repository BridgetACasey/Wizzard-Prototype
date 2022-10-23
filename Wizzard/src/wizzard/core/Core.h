//@BridgetACasey

#pragma once

#ifdef WIZZARD_PLATFORM_WINDOWS
	#ifdef WIZZARD_BUILD_DLL
		#define WIZZARD_API __declspec(dllexport)
	#else
		#define WIZZARD_API __declspec(dllimport)
	#endif
#else
#error Invalid platform detected - this software is only supported on Windows
#endif

#ifdef WIZZARD_ENABLE_ASSERTS
#define APP_ASSERT(x, ...) {if(!(x)) {APP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#define WIZ_ASSERT(x, ...) {if(!(x), {WIZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
#define APP_ASSERT(x, ...)
#define WIZ_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define WIZ_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)