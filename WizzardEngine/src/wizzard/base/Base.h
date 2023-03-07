//@BridgetACasey

#pragma once

#include <memory>

#ifdef WIZZARD_PLATFORM_WINDOWS
#if WIZZARD_DYNAMIC_LINK
	#ifdef WIZZARD_BUILD_DLL
		#define WIZZARD_API __declspec(dllexport)
	#else
		#define WIZZARD_API __declspec(dllimport)
	#endif
#else
	#define WIZZARD_API
#endif
#else
	#error Invalid platform detected - this software is only supported on Windows
#endif

#ifdef WIZ_DEBUG
	#define WIZ_ENABLE_ASSERTS
#endif

#ifdef WIZZARD_ENABLE_ASSERTS
	#define APP_ASSERT(x, ...) {if(!(x)) {APP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define LUG_ASSERT(x, ...) {if(!(x)) {LUG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
	#define WIZ_ASSERT(x, ...) {if(!(x), {WIZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();}}
#else
	#define APP_ASSERT(x, ...)
	#define LUG_ASSERT(x, ...)
	#define WIZ_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define WIZ_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Wizzard
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}