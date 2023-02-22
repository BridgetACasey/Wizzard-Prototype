//@BridgetACasey

#include "wzpch.h"

#include <comdef.h>
#include "imgui.h"

#include "ImGuiScreenReading.h"
#include "screenreading/ScreenReaderSupport.h"

namespace Wizzard
{
	//Probably not the best way of arranging this, to review later
	bool ImGuiSR::Button(const std::string& buttonLabel, const ImVec2& sizeArg, const std::string& description, bool preferDesc)
	{
		WIZ_PROFILE_FUNCTION();

		if (ImGui::Button(buttonLabel.c_str(), sizeArg))
		{
			if (preferDesc && !description.empty())
				return ScreenReaderSupport::OutputAll(description);

			return ScreenReaderSupport::OutputAll(buttonLabel);
		}

		//if(ImGui::IsItemFocused())
		//{
		//	return ScreenReaderSupport::OutputAll(buttonLabel);
		//}

		return false;
	}
}