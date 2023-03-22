//@BridgetACasey

#include "wzpch.h"

#include "Buffer.h"
#include "Renderer.h"

#include "platform/opengl/OpenGLBuffer.h"

namespace Wizzard
{
	WizRef<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		WIZ_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    WIZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return WizRef<OpenGLVertexBuffer>::CreateRef(size);
		}

		WIZ_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	WizRef<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
	{
		WIZ_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    WIZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return WizRef<OpenGLVertexBuffer>::CreateRef(vertices, size);
		}

		WIZ_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	WizRef<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		WIZ_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    WIZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return WizRef<OpenGLIndexBuffer>::CreateRef(indices, size);
		}

		WIZ_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}