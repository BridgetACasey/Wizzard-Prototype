//@BridgetACasey

#include <Wizzard.h>
#include <wizzard/base/EntryPoint.h>

#include "EditorLayer.h"

namespace Wizzard
{
	class LuggageEditor : public Application
	{
	public:
		LuggageEditor() : Application("Editor Application")
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