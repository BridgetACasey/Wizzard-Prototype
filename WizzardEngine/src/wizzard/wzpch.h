//@BridgetACasey

#pragma once

#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>

#include "wizzard/core/Log.h"
#include "wizzard/debug/Instrumentor.h"

#ifdef WIZZARD_PLATFORM_WINDOWS
	#include <Windows.h>
	#include <windowsx.h>
#endif