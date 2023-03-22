//@BridgetACasey

#pragma once

#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Wizzard
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL,
			Vulkan,
			DX11,
			DX12
		};

		virtual void Init() = 0;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColor(const glm::vec4& color) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const WizRef<VertexArray>& vertexArray, uint32_t count) = 0;

		static API GetAPI() { return rendererAPI; }

	private:
		static API rendererAPI;
	};
}