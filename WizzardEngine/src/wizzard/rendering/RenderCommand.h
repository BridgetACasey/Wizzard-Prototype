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

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			rendererAPI->SetViewport(x, y, width, height);
		}

		static void SetClearColor(const glm::vec4& color)
		{
			rendererAPI->SetClearColor(color);
		}

		static void Clear()
		{
			rendererAPI->Clear();
		}

		static void DrawIndexed(const WizRef<VertexArray>& vertexArray, uint32_t count = 0)
		{
			rendererAPI->DrawIndexed(vertexArray, count);
		}

	private:
		static RendererAPI* rendererAPI;
	};
}