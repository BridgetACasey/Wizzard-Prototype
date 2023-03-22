//@BridgetACasey

#include "wzpch.h"

#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Wizzard
{
	WizRef<VertexArray> VertexArray::Create()
	{
		WIZ_PROFILE_FUNCTION();

		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    WIZ_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return WizRef<OpenGLVertexArray>::CreateRef();
		}

		WIZ_ASSERT(false, "Unknown RendererAPI!");

		return nullptr;
	}
}