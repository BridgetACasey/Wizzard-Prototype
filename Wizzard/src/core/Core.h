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

#define BIT(x) (1 << x)