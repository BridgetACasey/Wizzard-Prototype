//@BridgetACasey

#pragma once

//Custom functions that override or encapsulate features of ImGui for better integration and user control within the engine

#include "imgui.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

namespace Wizzard
{
	//struct UIWindow
	//{
	//	UIWindow(ImGuiContext* context, const char* name) : imguiWindow(context, name)
	//	{
	//	}
	//	~UIWindow();
	//
	//private:
	//	ImGuiWindow imguiWindow;
	//};
}