//@BridgetACasey

#include "wzpch.h"

#include "ImGuiScreenReading.h"
#include "imgui.h"
#include "Tolk.h"

#include <comdef.h>

namespace Wizzard
{
	//Probably not the best way of arranging this, to review later
	bool ImGuiSR::Button(const char* label, const wchar_t* description, const ImVec2& size_arg)
	{
		WIZ_PROFILE_FUNCTION();

		bool success = false;

		if(ImGui::Button(label, size_arg))
		{
			std::wstring output = (L"Selected");
			output += description;

			Tolk_Output(output.c_str(), true);
			success = true;
		}

		return success;
	}
}