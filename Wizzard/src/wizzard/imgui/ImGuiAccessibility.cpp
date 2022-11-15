//@BridgetACasey

#include "wzpch.h"

#include "ImGuiAccessibility.h"

#include "Tolk.h"

namespace Wizzard
{
	//Probably not the best way of arranging this, to review later
	bool ImGuiAccessibility::Button(const char* label, const wchar_t* description, bool& readDesc, const ImVec2& size_arg)
	{
		if(ImGui::Button(label, size_arg))
		{
			std::wstring output = (L"Selected");
			output += description;

			Tolk_Output(output.c_str(), true);
		}

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_None))
		{
			ImGui::SetTooltip((const char*)description);

			if (readDesc)
			{
				//	//Tolk_Output(L"Hello I am Mr Screen Reader Man");
				Tolk_Output(description, true);
				readDesc = false;
			}
		}
		else
			readDesc = true;

		return true;
	}
}