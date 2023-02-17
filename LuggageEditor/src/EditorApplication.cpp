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
			editorLayer = new EditorLayer();

			PushLayer(editorLayer);
		}

		~LuggageEditor()
		{
			PopLayer(editorLayer);
		}

	private:
		EditorLayer* editorLayer = nullptr;
	};

	Application* CreateApplication()
	{
		return new LuggageEditor();
	}
}