//@BridgetACasey

#include "wzpch.h"
#include "LayerStack.h"

namespace Wizzard
{
	LayerStack::LayerStack()
	{
		WIZ_PROFILE_FUNCTION();
	}

	LayerStack::~LayerStack()
	{
		WIZ_PROFILE_FUNCTION();

		for (Layer* layer : layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		WIZ_PROFILE_FUNCTION();

		layers.emplace(layers.begin() + layerInsertionIndex, layer);
		layerInsertionIndex++;
		layer->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		WIZ_PROFILE_FUNCTION();

		auto it = std::find(layers.begin(), layers.end(), layer);

		if (it != layers.end())
		{
			layers.erase(it);
			layerInsertionIndex--;
			layer->OnDetach();
		}
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		WIZ_PROFILE_FUNCTION();

		layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		WIZ_PROFILE_FUNCTION();

		auto it = std::find(layers.begin(), layers.end(), overlay);

		if (it != layers.end())
		{
			layers.erase(it);
			overlay->OnDetach();
		}
	}
}