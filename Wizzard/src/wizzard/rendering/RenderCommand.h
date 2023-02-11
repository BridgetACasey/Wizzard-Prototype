//@BridgetACasey

#pragma once

#include "RendererAPI.h"

namespace Wizzard
{
	class RenderCommand
	{
	public:
		static void Init()
		{
			rendererAPI->Init();
		}

		static void SetClearColor(const glm::vec4& color)
		{
			rendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			rendererAPI->Clear();
		}

		static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			rendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static RendererAPI* rendererAPI;
	};
}