//@BridgetACasey

#include <Wizzard.h>
#include <wizzard/base/EntryPoint.h>

#include "EditorLayer.h"

namespace Wizzard
{
	class LuggageEditor : public Application
	{
	public:
		LuggageEditor() : Application("Luggage Editor")
		{
			PushLayer(new EditorLayer());
		}

		~LuggageEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new LuggageEditor();
	}
}