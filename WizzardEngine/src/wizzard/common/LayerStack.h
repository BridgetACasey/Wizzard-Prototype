//@BridgetACasey

#pragma once

#include "wizzard/base/Base.h"
#include "Layer.h"

#include <vector>

namespace Wizzard
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);

		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return layers.begin(); }
		std::vector<Layer*>::iterator end() { return layers.end(); }

	private:
		std::vector<Layer*> layers;
		unsigned int layerInsertionIndex = 0;
	};
}